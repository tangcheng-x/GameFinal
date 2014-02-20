#include "stdafx.h"
#include "CResourceFactory.h"
#include "CSimpleMesh.h"
#include "CModelMesh.h"
#include "CAnimatedMesh.h"

ISimpleMesh* CResourceFactory::createSimpleMesh(
	const std::string& name,
	u32 sortcode,
	void* vertices,
	void* indices,
	u32 vertexCount,
	u32 vertexStride,
	u32 indiceCount,
	bool bit32Index,
	E_MEMORY_USAGE usage)
{
	IMeshBuffer* buffer = createMeshBuffer(usage, vertices, indices, vertexCount, indiceCount, vertexStride, bit32Index);
	if (!buffer)
	{
		GF_PRINT_CONSOLE_INFO("The mesh ('%s') create failed!. Due to the failure of mesh buffer.\n", name.c_str());
		return nullptr;
	}
	
	CSimpleMesh* mesh = new CSimpleMesh(name, sortcode, buffer);
	if (!mesh)
	{
		GF_PRINT_CONSOLE_INFO("The mesh ('%s') create failed!. Due to the lack of memory.\n", name.c_str());
		buffer->drop();
		return nullptr;
	}

	return mesh;
}

IModelMesh* CResourceFactory::createModelMesh(
	const std::string& name,
	u32 sortcode,
	void* vertices,
	void* indices,
	u32 vertexCount,
	u32 vertexStride,
	u32 indiceCount,
	bool bit32Index,
	const std::vector<IModelMesh::SModelSubset>& subsets,
	E_MEMORY_USAGE usage)
{
	IMeshBuffer* buffer = createMeshBuffer(usage, vertices, indices, vertexCount, indiceCount, vertexStride, bit32Index);
	if (!buffer)
	{
		GF_PRINT_CONSOLE_INFO("The mesh ('%s') create failed!. Due to the failure of mesh buffer.\n", name.c_str());
		return nullptr;
	}

	if (subsets.size() < 1)
	{
		GF_PRINT_CONSOLE_INFO("FAIL:The mesh ('%s') create failed!. The subsets count must be greater than 1.\n", name.c_str());
		return nullptr;
	}

	CModelMesh* mesh = new CModelMesh(name, sortcode, subsets, buffer);
	if (!mesh)
	{
		GF_PRINT_CONSOLE_INFO("The mesh ('%s') create failed!. Due to the lack of memory.\n", name.c_str());
		buffer->drop();
		return nullptr;
	}

	return mesh;
}

IAnimatedMesh* CResourceFactory::createAnimatedModelMesh(
	const std::string& name,
	u32 sortcode,
	void* vertices,
	void* animateVertices,
	void* indices,
	u32 vertexCount,
	u32 animateVertexCount,
	u32 indicesCount,
	u32 vertexStride,
	u32 animateVertexStride,
	bool bit32Index,
	E_MEMORY_USAGE usage,
	const std::vector<SAnimatedModelSubset>& subsets,
	const std::vector<SModelBone>& bones,
	const std::vector<SBoneAnimationClip>& animateClips)
{
	IAnimatedMeshBuffer* buffer = createAnimatedMeshBuffer(usage, vertices, animateVertices,
		indices, vertexCount, animateVertexCount, indicesCount,
		vertexStride, animateVertexStride, bit32Index);

	if (!buffer)
	{
		GF_PRINT_CONSOLE_INFO("The mesh ('%s') create failed!. Due to the failure of mesh buffer.\n", name.c_str());
		return nullptr;
	}

	if (subsets.size() < 1)
	{
		GF_PRINT_CONSOLE_INFO("FAIL:The mesh ('%s') create failed!. The subsets count must be greater than 1.\n", name.c_str());
		return nullptr;
	}

	CAnimatedMesh* mesh = new CAnimatedMesh(name, sortcode, subsets, bones, animateClips, buffer);

	//CModelMesh* mesh = new CModelMesh(name, sortcode, subsets, buffer);
	if (!mesh)
	{
		GF_PRINT_CONSOLE_INFO("The mesh ('%s') create failed!. Due to the lack of memory.\n", name.c_str());
		buffer->drop();
		return nullptr;
	}

	return mesh;

}

