#include "stdafx.h"
#include "gfUtil.h"
#include "CSceneManager.h"
#include "CMeshNode.h"
#include "CAnimatedMeshNode.h"
#include "CLightNode.h"
#include "CCameraNode.h"
#include "CFpsCameraNode.h"
#include "CModelMeshNode.h"

CSceneManager::CSceneManager(IDevice* device) 
	:ISceneManager(device)
{
	//get Video Driver object
	mVideoDriver = device->getVideoDriver();

	//get Resource Factory
	IResourceFactory* pResourceFactory = mVideoDriver->getResourceFactory();

	mLightNodes.fill(nullptr);
	mCameraNodes.fill(nullptr);
	mActiveCameraId = EMPTY_CAMERA_ID;
}

CSceneManager::~CSceneManager()
{
	//ReleaseReferenceCounted(mGeometryCreator);
}

IMeshNode* CSceneManager::addMeshNode(
	ISimpleMesh* mesh,
	IMaterial* material,
	ISceneNode* parent,
	const XMFLOAT3& position,
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale)
{
	if (!mesh)
		return nullptr;
	
	if (!parent)
		parent = this;

	IMeshNode* node = new CMeshNode(parent, this, mesh, material, position, rotation, scale);

	return node;
}

IMeshNode* CSceneManager::addModelMeshNode(
	IModelMesh* mesh,
	ISceneNode* parent,
	const XMFLOAT3& position,
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale)
{
	if (!mesh)
		return nullptr;

	if (!parent)
		parent = this;

	IMeshNode* node = new CModelMeshNode(parent, this, mesh, position, rotation, scale);

	return node;
}

IAnimatedMeshNode* CSceneManager::addAnimatedMeshNode(
	IAnimatedMesh* mesh,
	ISceneNode* parent,
	const XMFLOAT3& position,
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale)
{
	if (!mesh)
		return nullptr;

	if (!parent)
		parent = this;

	IAnimatedMeshNode* node = new CAnimatedMeshNode(parent, this, mesh, position, rotation, scale);

	return node;
}

void CSceneManager::collectMeshNodeShaders(IMeshNode* node)
{
	// collect all the shaders ( for per-frame variables optimization)
	u32 subsetCount = node->getSubsetCount();
	for (u32 i = 0; i < subsetCount; i++)
	{
		IMaterial* material = node->getMaterial(i);
		if (material)
		{
			for (u32 j = 0; j < material->getPipelineCount(); j++)
			{
				IPipeline* pipeline = material->getPipeline(j);
				if (pipeline)
				{
					for (u32 k = 0; k < EST_SHADER_COUNT; k++)
					{
						IShader* shader = pipeline->getShader((E_SHADER_TYPE)k);
						if (shader != nullptr)
							mActiveShaders.insert(shader);
					}
				}
			}
		}
	}
}


void CSceneManager::registerNodeForRendering(IMeshNode* node, E_NODE_TYPE nodeType)
{
	if (ENT_SOLID_NODE)
	{
		if (!isCulled(node))
		{
			collectMeshNodeShaders(node);
			mSolidNodes.push_back(node);
		}
	}
}

void CSceneManager::render()
{

}

void CSceneManager::drawAll()
{
	OnAnimate(0);

	mSolidNodes.clear();
	mActiveShaders.clear();

	OnRegisterSceneNode();

	/* reset all the active shaders. (for per-frame variables optimizations.) */
	for (auto it = mActiveShaders.begin(); it != mActiveShaders.end(); it++)
	{
		(*it)->reset();
	}


	/* sort all the solid nodes according to the sort code. */
	std::sort(mSolidNodes.begin(), mSolidNodes.end(), [](ISceneNode* node1, ISceneNode* node2){
		return node1->getSortCode() < node2->getSortCode();
	});

	/* render all the solid nodes */
	auto it = mSolidNodes.begin();
	for (; it != mSolidNodes.end(); it++)
	{
		(*it)->render();
	}
}


ILightNode* CSceneManager::addLightNode(u32 id, ISceneNode* parent,
	const XMFLOAT3& position)
{
	if (parent == nullptr)
		parent = this;

	if (id < 0 || id >= MAX_LIGHT_COUNT)
	{
		GF_PRINT_CONSOLE_INFO("The light id must between 0 and %d.\n", 0, MAX_LIGHT_COUNT - 1);
		return nullptr;
	}

	if (mLightNodes[id] != nullptr)
	{
		GF_PRINT_CONSOLE_INFO("The light with id %d already existed!\n", id);
		return nullptr;
	}

	ILightNode* pLight = new CLightNode(id, parent, this, position);
	mLightNodes[id] = pLight;
	return pLight;
}

ILightNode* CSceneManager::getLightNode(u32 id)
{
	if (id < 0 || id >= MAX_LIGHT_COUNT)
	{
		GF_PRINT_CONSOLE_INFO("The light id must between 0 and %d.\n", 0, MAX_LIGHT_COUNT - 1);
		return nullptr;
	}

	return mLightNodes[id];
}

ICameraNode* CSceneManager::addCameraNode(u32 id,
	ISceneNode* parent,
	const XMFLOAT3& position,
	const XMFLOAT3& lookat,
	const XMFLOAT3& up,
	f32 fov,
	f32 nearZ,
	f32 farZ,
	f32 aspectRatio)
{
	if (parent == nullptr)
		parent = this;

	if (id <= 0 || id >= MAX_CAMERA_COUNT)
	{
		GF_PRINT_CONSOLE_INFO("The camera id must between 1 and %d.\n", 0, MAX_CAMERA_COUNT - 1);
		return nullptr;
	}

	if (mCameraNodes[id] != nullptr)
	{
		GF_PRINT_CONSOLE_INFO("The camera with id %d already existed!\n", id);
		return nullptr;
	}

	/* if the aspectRatio is the default param. */
	if (aspectRatio < 0)
	{
		f32 width = static_cast<f32>(mDevice->getClientWidth());
		f32 height = static_cast<f32>(mDevice->getClientHeight());
		aspectRatio = width / height;
	}

	ICameraNode* camera = new CCameraNode(parent, this, position,
		lookat, up, aspectRatio, fov, nearZ, farZ);
	mCameraNodes[id] = camera;

	if (mActiveCameraId == EMPTY_CAMERA_ID)
		setActiveCamera(id);

	return camera;
}

IFpsCameraNode* CSceneManager::addFpsCameraNode(u32 id,
	ISceneNode* parent,
	const XMFLOAT3& position,
	const XMFLOAT3& lookat,
	const XMFLOAT3& up,
	f32 maxUpAngle,
	f32 maxDownAngle,
	f32 fov,
	f32 nearZ,
	f32 farZ,
	f32 aspectRatio)
{
	if (parent == nullptr)
		parent = this;

	if (id <= 0 || id >= MAX_CAMERA_COUNT)
	{
		GF_PRINT_CONSOLE_INFO("The camera id must between 1 and %d.\n", 0, MAX_CAMERA_COUNT - 1);
		return nullptr;
	}

	if (mCameraNodes[id] != nullptr)
	{
		GF_PRINT_CONSOLE_INFO("The camera with id %d already existed!\n", id);
		return nullptr;
	}

	/* if the aspectRatio is the default param. */
	if (aspectRatio < 0)
	{
		f32 width = static_cast<f32>(mDevice->getClientWidth());
		f32 height = static_cast<f32>(mDevice->getClientHeight());
		aspectRatio = width / height;
	}

	IFpsCameraNode* camera = new CFpsCameraNode(parent, this, position, lookat, up, aspectRatio,
		fov, nearZ, farZ, maxUpAngle, maxDownAngle);

	mCameraNodes[id] = camera;
	if (mActiveCameraId == EMPTY_CAMERA_ID)
		setActiveCamera(id);

	return camera;
}

ICameraNode* CSceneManager::getActiveCameraNode()
{
	return mCameraNodes[0];
}

u32 CSceneManager::getActiveCameraId() const
{
	return mActiveCameraId;
}

ICameraNode* CSceneManager::getCameraNode(u32 id)
{
	if (id >= MAX_CAMERA_COUNT)
		return nullptr;

	return mCameraNodes[id];
}

ICameraNode* CSceneManager::setActiveCamera(u32 id)
{
	if (id >= MAX_CAMERA_COUNT || !mCameraNodes[id])
		return nullptr;

	mActiveCameraId = id;
	mCameraNodes[0] = mCameraNodes[id];
	return mCameraNodes[0];
}

bool CSceneManager::setActiveCamera(ICameraNode* camera)
{
	if (!camera)
		return false;

	for (u32 i = 1; i < MAX_CAMERA_COUNT; i++)
	{
		if (mCameraNodes[i] == camera)
		{
			mActiveCameraId = i;
			mCameraNodes[0] = camera;
			return true;
		}
	}

	return false;
}

