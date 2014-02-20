#include "stdafx.h"
#include "gfUtil.h"
#include "CModelFileParser.h"

bool CModelFileParser::parseModelFile(const std::string& filepath, SModelMeshCreateParams& createParams)
{
	FILE* fp = fopen(filepath.c_str(), "rb");
	if (!fp)
	{
		GF_PRINT_CONSOLE_INFO("The mesh file '%s' doesn't exist.\n", filepath.c_str());
		return false;
	}


	fread(&createParams.Header, sizeof(SModelFileHeader), 1, fp);

	u32 subsetCount = createParams.Header.SubsetCount;
	createParams.Subsets.resize(subsetCount);

	fread(&createParams.Subsets[0], sizeof(SModelFileSubset), subsetCount, fp);

	if (createParams.Header.AnimatedMesh)
	{
		/* 读取每个子集的骨骼 */
		createParams.SubsetBones.resize(subsetCount);

		for (u32 i = 0; i < subsetCount; i++)
		{
			SModelFileSubset& subset = createParams.Subsets[i];
			/* 如果该子集含有骨骼动画 */
			if (subset.Skinned)
			{
				createParams.SubsetBones[i].resize(subset.BoneCount);
				fread(&createParams.SubsetBones[i][0], sizeof(SModelSubsetBone), subset.BoneCount, fp);
			}
		}

		/* 读取骨骼树 */
		createParams.Bones.resize(createParams.Header.BoneCount);
		fread(&createParams.Bones[0], sizeof(SModelBone), createParams.Header.BoneCount, fp);

		/* 读取AnimationClip */
		createParams.AnimationClips.resize(createParams.Header.AnimationClipCount);
		for (u32 i = 0; i < createParams.Header.AnimationClipCount; i++)
		{
			SBoneAnimationClip& animationClip = createParams.AnimationClips[i];
			/* 读取该AnimationClip的Name */
			fread(animationClip.Name, sizeof(animationClip.Name), 1, fp);
			
			/* 读取AnimationClip所含有的Bone个数 */
			u16 boneCount;
			fread(&boneCount, sizeof(u16), 1, fp);

			animationClip.BoneAnimations.resize(boneCount);

			for (u32 j = 0; j < boneCount; j++)
			{
				u16 boneId, translateFrameCount, scaleFrameCount, rotateFrameCount;
				fread(&boneId, sizeof(u16), 1, fp);
				fread(&translateFrameCount, sizeof(u16), 1, fp);
				fread(&scaleFrameCount, sizeof(u16), 1, fp);
				fread(&rotateFrameCount, sizeof(u16), 1, fp);
			
				SBoneAnimation& boneAnimation = animationClip.BoneAnimations[j];
				boneAnimation.BoneID = boneId;
				boneAnimation.TranslationFrames.resize(translateFrameCount);
				boneAnimation.ScaleFrames.resize(scaleFrameCount);
				boneAnimation.RotationFrames.resize(rotateFrameCount);
				
				fread(&boneAnimation.TranslationFrames[0], sizeof(STranslationAnimateFrame), translateFrameCount, fp);
				fread(&boneAnimation.ScaleFrames[0], sizeof(SScaleAnimateFrame), scaleFrameCount, fp);
				fread(&boneAnimation.RotationFrames[0], sizeof(SRotationAnimateFrame), rotateFrameCount, fp);
			}
		}
	}


	if (createParams.Header.VertexBufferSize > 0)
	{
		createParams.VertexBuffer = malloc(createParams.Header.VertexBufferSize);
		fread(createParams.VertexBuffer, createParams.Header.VertexBufferSize, 1, fp);
	}

	if (createParams.Header.AnimateVertexBufferSize > 0)
	{
		createParams.AnimateVertexBuffer = malloc(createParams.Header.AnimateVertexBufferSize);
		fread(createParams.AnimateVertexBuffer, createParams.Header.AnimateVertexBufferSize, 1, fp);
	}
	
	createParams.IndexBuffer = malloc(createParams.Header.IndexBufferSize);
	fread(createParams.IndexBuffer, createParams.Header.IndexBufferSize, 1, fp);

	fclose(fp);

	return true;
}


