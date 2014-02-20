#include "stdafx.h"
#include "CMeshManager.h"

CMeshManager::CMeshManager(IResourceFactory* pResourceFactory, IGeometryCreator* pGeometryCreator)
: mResourceFactory(pResourceFactory)
, mGeometryCreator(pGeometryCreator)
{
	
}

ISimpleMesh* CMeshManager::createSimpleMesh(
	const std::string& name,
	void* vertices,
	void* indices,
	u32 vertexCount,
	u32 vertexStride,
	u32 indiceCount,
	bool bit32Index,
	E_MEMORY_USAGE usage)
{
	auto it = mMeshMap.find(name);
	if (it != mMeshMap.end())
	{
		if (it->second->getType() == EMT_SIMPLE_MESH)
		{
			GF_PRINT_CONSOLE_INFO("WARNING: The simple mesh named '%s' has already existed.\n", name.c_str());
			return dynamic_cast<ISimpleMesh*>(it->second);
		}
		else
		{
			GF_PRINT_CONSOLE_INFO("WARNING: The mesh named '%s' has already existed. \
				Fortheremore it is not a simple mesh.\n", name.c_str());
			return nullptr;
		}
	}

	u32 sortcode = mCodeAllocator.allocate();
	ISimpleMesh* mesh = mResourceFactory->createSimpleMesh(name, sortcode, vertices, indices,
		vertexCount, vertexStride, indiceCount, bit32Index, usage);

	if (!mesh)
	{
		GF_PRINT_CONSOLE_INFO("ERROR:The mesh named '%s' created failed!\n", name.c_str());
		mCodeAllocator.release(sortcode);
		return nullptr;
	}

	mMeshMap.insert(std::make_pair(name, mesh));
	return mesh;
}

IModelMesh* CMeshManager::createModelMesh(const std::string& name,
	void* vertices,
	void* indices,
	u32 vertexCount,
	u32 vertexStride,
	u32 indiceCount,
	const std::vector<IModelMesh::SModelSubset>& subsets,
	bool bit32Index,
	E_MEMORY_USAGE usage)
{
	auto it = mMeshMap.find(name);

	if (it != mMeshMap.end())
	{
		if (it->second->getType() == EMT_MODEL_MESH)
		{
			GF_PRINT_CONSOLE_INFO("WARNING: The model mesh named '%s' has already existed.\n", name.c_str());
			return dynamic_cast<IModelMesh*>(it->second);
		}
		else
		{
			GF_PRINT_CONSOLE_INFO("WARNING: The mesh named '%s' has already existed. \
				Fortheremore it is not a model mesh.\n", name.c_str());
			return nullptr;
		}
	}

	u32 sortcode = mCodeAllocator.allocate();
	IModelMesh* mesh = mResourceFactory->createModelMesh(name, sortcode, vertices, indices,
		vertexCount, vertexStride, indiceCount, bit32Index, subsets, usage);

	if (!mesh)
	{
		GF_PRINT_CONSOLE_INFO("ERROR:The mesh named '%s' created failed!\n", name.c_str());
		mCodeAllocator.release(sortcode);
		return nullptr;
	}

	mMeshMap.insert(std::make_pair(name, mesh));
	return mesh;
}


IMesh* CMeshManager::getMesh(const std::string& name)
{
	auto it = mMeshMap.find(name);
	if (it == mMeshMap.end())
		return nullptr;

	return it->second;
}

ISimpleMesh* CMeshManager::getSimpleMesh(const std::string& name)
{
	auto it = mMeshMap.find(name);
	if (it == mMeshMap.end())
		return nullptr;

	if (it->second->getType() != EMT_SIMPLE_MESH)
		return nullptr;

	return dynamic_cast<ISimpleMesh*>(it->second);
}

IModelMesh* CMeshManager::getModelMesh(const std::string& name)
{
	auto it = mMeshMap.find(name);
	if (it == mMeshMap.end())
		return nullptr;

	if (it->second->getType() != EMT_MODEL_MESH)
		return nullptr;

	return dynamic_cast<IModelMesh*>(it->second);
}


IAnimatedMesh* CMeshManager::getAnimatedMesh(const std::string& name)
{
	auto it = mMeshMap.find(name);
	if (it == mMeshMap.end())
		return nullptr;

	if (it->second->getType() != EMT_ANIMATE_MODEL_MESH)
		return nullptr;

	return dynamic_cast<IAnimatedMesh*>(it->second);
}

ISimpleMesh* CMeshManager::createCubeMesh(
	const std::string& name,
	f32 width,
	f32 height,
	f32 depth,
	E_MEMORY_USAGE usage)
{
	SGeometryData geoData;
	mGeometryCreator->createCubeData(width, height, depth, geoData);

	ISimpleMesh* mesh = createSimpleMesh(name, &geoData.Vertices[0], &geoData.Indices[0],
		geoData.Vertices.size(), mGeometryCreator->getVertexStride(), geoData.Indices.size(),
		false, usage);
	return mesh;
}


ISimpleMesh* CMeshManager::createPlaneMesh(
	const std::string& name,
	f32 width,
	f32 depth,
	u32 xsegments,
	u32 ysegments,
	f32 uTiles,
	f32 vTiles,
	E_MEMORY_USAGE usage)
{
	
	if (xsegments == 0 || ysegments == 0)
	{
		GF_PRINT_CONSOLE_INFO("The segments of a plane mesh can't be 0.\n");
		return nullptr;
	}

	SGeometryData geoData;
	mGeometryCreator->createPlaneData(width, depth, xsegments, ysegments, uTiles, vTiles, geoData);

	bool bit32Index = false;
	if (geoData.Indices.size() >= 0xffff)
		bit32Index = true;

	ISimpleMesh* mesh = createSimpleMesh(name, &geoData.Vertices[0], &geoData.Indices[0],
		geoData.Vertices.size(), mGeometryCreator->getVertexStride(), geoData.Indices.size(),
		bit32Index, usage);

	return mesh;
}

IAnimatedMesh* CMeshManager::createAnimatedModelMesh(
	const std::string& name,
	void* vertices,
	void* animateVertices,
	void* indices,
	bool bit32Index,
	u32 vertexCount,
	u32 animateVertexCount,
	u32 indicesCount,
	u32 vertexStride,
	u32 animateVertexStride,
	E_MEMORY_USAGE usage,
	const std::vector<SModelBone>& bones,
	const std::vector<SAnimatedModelSubset>& subsets,
	const std::vector<SBoneAnimationClip>& animateClips)
{
	auto it = mMeshMap.find(name);

	if (it != mMeshMap.end())
	{
		if (it->second->getType() == EMT_ANIMATE_MODEL_MESH)
		{
			GF_PRINT_CONSOLE_INFO("WARNING: The animated mesh named '%s' has already existed.\n", name.c_str());
			return dynamic_cast<IAnimatedMesh*>(it->second);
		}
		else
		{
			GF_PRINT_CONSOLE_INFO("WARNING: The mesh named '%s' has already existed. \
								  				Fortheremore it is not a animated mesh.\n", name.c_str());
			return nullptr;
		}
	}

	u32 sortcode = mCodeAllocator.allocate();
	IAnimatedMesh* mesh = mResourceFactory->createAnimatedModelMesh(name, sortcode, vertices,
		animateVertices, indices, vertexCount, animateVertexCount, indicesCount, vertexStride, animateVertexStride,
		bit32Index, usage, subsets, bones, animateClips);

	if (!mesh)
	{
		GF_PRINT_CONSOLE_INFO("ERROR:The mesh named '%s' created failed!\n", name.c_str());
		mCodeAllocator.release(sortcode);
		return nullptr;
	}

	mMeshMap.insert(std::make_pair(name, mesh));
	return mesh;

}
