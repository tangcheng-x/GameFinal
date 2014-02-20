#ifndef __MODEL_FILE_PARSER_INTERFACE_H__
#define __MODEL_FILE_PARSER_INTERFACE_H__

#include "IReferenceCounted.h"
#include "IAnimatedMesh.h"
#include "gfMath.h"

enum E_MODEL_VERTEX_ELEMENT
{
	EMVE_POSITION = 1,
	EMVE_NORMAL = (1 << 1),
	EMVE_TANGENT = (1 << 2),
	EMVE_BINORMAL = (1 << 3),
	EMVE_TEXCOORD = (1 << 4)
};


struct SModelFileHeader
{
	u32							Version;
	u32							VertexElements;
	SAxisAlignedBox				Aabb;
	u32							VertexCount;
	u32							AnimateVertexCount;
	u32							IndiceCount;
	/* 不带bone_indices和weights两个字段的stride，用于有动画的子集*/
	u32							VertexStride;
	u32							AnimateVertexStride; 
	u32							VertexBufferSize;
	u32							AnimateVertexBufferSize;
	u32							IndexBufferSize;
	u32							SubsetCount;
	/* 带有动画的子集个数 */
	u32							AnimateSubsetCount;
	u32							AnimationClipCount;
	u32							BoneCount;
	u32							BoneWeightCount; /* 最大为4 */
	bool						AnimatedMesh;
	bool						Bit32Indice;
};

struct SModelFileSubset
{
	u32							BaseVertexLocation;
	u32							StartIndexLocation;
	u32							IndexCount;
	u32							BoneCount;
	s32							BoneId;					/* 如果在骨骼动画模型中，该子集没有骨骼动画，则需要经过这个骨骼的变换 */
	bool						Skinned;
	char						MaterialName[128];
};


class IModelFileParser : public IReferenceCounted
{
public:
	struct SModelMeshCreateParams
	{
		SModelFileHeader								Header;
		std::vector<SModelFileSubset>					Subsets;
		std::vector<SModelBone>							Bones;
		std::vector<std::vector<SModelSubsetBone>>		SubsetBones;
		std::vector<SBoneAnimationClip>					AnimationClips;
		void*											VertexBuffer;
		void*											AnimateVertexBuffer;
		void*											IndexBuffer;
		
		SModelMeshCreateParams()
		{
			VertexBuffer = nullptr;
			AnimateVertexBuffer = nullptr;
			IndexBuffer = nullptr;
		}

		~SModelMeshCreateParams()
		{
			free(VertexBuffer);
			free(AnimateVertexBuffer);
			free(IndexBuffer);
		}
	};
	
public:

	virtual bool parseModelFile(const std::string& filepath, SModelMeshCreateParams& createParams) = 0;

};

#endif
