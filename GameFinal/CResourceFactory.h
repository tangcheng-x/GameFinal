#ifndef __RESOURCE_FACTORY_CLASS_H__
#define __RESOURCE_FACTORY_CLASS_H__

#include "IResourceFactory.h"

class CResourceFactory : public IResourceFactory
{
public:
	virtual ISimpleMesh* createSimpleMesh(
		const std::string& name,
		u32 sortcode,
		void* vertices,
		void* indices,
		u32 vertexCount,
		u32 vertexStride,
		u32 indiceCount,
		bool bit32Index,
		E_MEMORY_USAGE usage = EMU_STATIC);

	virtual IModelMesh* createModelMesh(
		const std::string& name,
		u32 sortcode,
		void* vertices,
		void* indices,
		u32 vertexCount,
		u32 vertexStride,
		u32 indiceCount,
		bool bit32Index,
		const std::vector<IModelMesh::SModelSubset>& subsets,
		E_MEMORY_USAGE usage = EMU_STATIC);

	virtual IAnimatedMesh* createAnimatedModelMesh(
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
		const std::vector<SBoneAnimationClip>& animateClips);

};

#endif
