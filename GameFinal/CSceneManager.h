#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__

#include "ISceneManager.h"


class CSceneManager : public ISceneManager
{
public:
	CSceneManager(IDevice* device);

	virtual ~CSceneManager();


	virtual void drawAll();

	virtual void render();

	virtual IMeshNode* addMeshNode(
		ISimpleMesh* mesh,
		IMaterial* material,
		ISceneNode* parent = nullptr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f));

	virtual IMeshNode* addModelMeshNode(
		IModelMesh* mesh,
		ISceneNode* parent = nullptr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f));

	virtual IAnimatedMeshNode* addAnimatedMeshNode(
		IAnimatedMesh* mesh,
		ISceneNode* parent = nullptr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f));

	virtual void registerNodeForRendering(IMeshNode* node, E_NODE_TYPE nodeType = ENT_SOLID_NODE);

	virtual ILightNode* addLightNode(u32 id, ISceneNode* parent = nullptr, const XMFLOAT3& position = XMFLOAT3(0, 0, 0));
	
	virtual ICameraNode* addCameraNode(u32 id,
		ISceneNode* parent = nullptr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& lookat = XMFLOAT3(0, 0, 5.0f),
		const XMFLOAT3& up = XMFLOAT3(0, 1.0f, 0),
		f32 fov = XM_PIDIV4,
		f32 nearZ = 1.0f,
		f32 farZ = 1000.0f,
		f32 aspectRatio = -1.0f);

	virtual IFpsCameraNode* addFpsCameraNode(u32 id,
		ISceneNode* parent = nullptr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, -5.0f),
		const XMFLOAT3& lookat = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& up = XMFLOAT3(0, 1.0f, 0),
		f32 maxUpAngle = XM_PI * 0.33f,
		f32 maxDownAngle = XM_PI * 0.33f,
		f32 fov = XM_PIDIV4,
		f32 nearZ = 1.0f,
		f32 farZ = 1000.0f,
		f32 aspectRatio = -1.0f);

	virtual ILightNode* getLightNode(u32 id);

	virtual ICameraNode* getActiveCameraNode();

	virtual u32 getActiveCameraId() const;

	virtual ICameraNode* getCameraNode(u32 id);

	virtual ICameraNode* setActiveCamera(u32 id);

	virtual bool setActiveCamera(ICameraNode* camera);

	bool isCulled(ISceneNode* node)
	{
		return false;
	}

private:
	void collectMeshNodeShaders(IMeshNode* node);

	std::vector<ISceneNode*>		mSolidNodes;
	std::array<ILightNode*, 1024>	mLightNodes;
	std::array<ICameraNode*, 65>	mCameraNodes;
	std::set<IShader*>				mActiveShaders;

	ICameraNode*					mActiveCamera;
	u32								mActiveCameraId;
};

#endif