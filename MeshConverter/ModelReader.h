#ifndef __MODEL_READER_H__
#define __MODEL_READER_H__

#include <iostream>
#include <Windows.h>
#include <GameFinal.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/material.h>


const u32 MATERIAL_NAME_MAX_LENGTH = 128;
const f32 INF_FLOAT = 1000000.0f;
const u32 BONE_NAME_MAX_LENGTH = 32;
const u32 ANIMATION_CLIP_NAME_MAX_LENGTH = 32;

struct SModelMaterial
{
	char							Name[MATERIAL_NAME_MAX_LENGTH];
	XMFLOAT4						Ambient;
	XMFLOAT4						Diffuse;
	XMFLOAT4						Specular;
	XMFLOAT4						Emissive;
	std::vector<std::string>		TextureNames;
};

struct SVertexSkinInfo
{
	u32					BoneId;
	f32					Weight;
};

struct SModelSubsetWrapper
{
	SModelFileSubset								Subset;
	std::vector<SModelSubsetBone>					SubsetBones;

	/* 顶点对应的bone id和weight
	比如:
	VertexSkinInfos[4][0].BoneId = 8;  VertexSkinInfos[4][1].Weight = 0.3;
	VertexSkinInfos[4][0].BoneId = 13; VertexSkinInfos[4][1].Weight = 0.7;

	就表示：顶点4同时附着在8号和13号两块骨骼上，并且附着重量分别是0.3和0.7
	*/
	std::vector<std::vector<SVertexSkinInfo>>		VertexSkinInfos;
};

struct SModelBoneWrapper
{
	SModelBone					Bone;
	s32							SubsetId;
};

void loadMaterials(const aiScene* scene,
	const std::string& file_name_without_ext,
	const std::string& file_ext,
	std::vector<SModelMaterial>& materials);


bool parseFilePath(const std::string& fullpath, 
	std::string& filename, 
	std::string& name_without_ext, 
	std::string& ext);

void fillSubsets(const aiScene* scene,
	const std::vector<SModelMaterial>& materials,
	SModelFileHeader& file_header,
	std::vector<SModelSubsetWrapper>& subsets_wrappers);

void calcVertexStride(SModelFileHeader& header);

void fillVertexAndIndicesData(const aiScene* scene,
	SModelFileHeader& header,
	f32 mesh_scale,
	const std::vector<SModelSubsetWrapper>& subset_wrappers,
	u8* pVertices,
	u8* pAnimateVertices,
	u8* pIndices);

void buildBoneTree(std::vector<SModelBoneWrapper>& bones, const aiNode* node, s32 parent);

void fillSubsetsBones(std::vector<SModelSubsetWrapper>& subset_wrappers, 
	const std::vector<SModelBoneWrapper>& bones, 
	SModelFileHeader& header,
	const aiScene* scene);

s32 getBoneIndexByName(const std::vector<SModelBoneWrapper>& bones, const char* name);

s32 getBoneIndexBySubsetId(const std::vector<SModelBoneWrapper>& bones, u32 id);

void loadAnimations(SModelFileHeader& header, 
	std::vector<SBoneAnimationClip>& animationClips, 
	const std::vector<SModelBoneWrapper>& bones,
	const aiScene* scene);

XMFLOAT4X4 convertFromAiMatrix(aiMatrix4x4 m);

bool isReallyAnimatedMesh(const aiScene* scene);

#endif