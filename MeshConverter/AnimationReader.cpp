#include "ModelReader.h"

XMFLOAT4X4 convertFromAiMatrix(aiMatrix4x4 m)
{
	return XMFLOAT4X4(
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4);
}

void buildBoneTree(std::vector<SModelBoneWrapper>& bones, const aiNode* node, s32 parent)
{
	SModelBoneWrapper bone;
	strcpy_s(bone.Bone.Name, node->mName.C_Str());
	bone.Bone.Parent = parent;
	bone.Bone.Transform = convertFromAiMatrix(node->mTransformation);

	if (node->mNumMeshes > 0)
		bone.SubsetId = node->mMeshes[0];
	else
		bone.SubsetId = -1;

	bones.push_back(bone);
	int nodeIndex = bones.size() - 1;

	for (u32 i = 0; i < node->mNumChildren; i++)
	{
		buildBoneTree(bones, node->mChildren[i], nodeIndex);
	}
}

void fillSubsetsBones(std::vector<SModelSubsetWrapper>& subset_wrappers,
	const std::vector<SModelBoneWrapper>& bones, 
	SModelFileHeader& header,
	const aiScene* scene)
{
	subset_wrappers.resize(header.SubsetCount);

	for (u32 i = 0; i < header.SubsetCount; i++)
	{
		SModelSubsetWrapper& wrapper = subset_wrappers[i];

		aiMesh* mesh = scene->mMeshes[i];
		u32 boneCount = mesh->mNumBones;
		u32 vertexCount = mesh->mNumVertices;
		wrapper.Subset.BoneCount = boneCount;

		/* 如果该子集含有骨骼动画 */
		if (mesh->HasBones())
		{
			wrapper.SubsetBones.resize(boneCount);
			wrapper.VertexSkinInfos.resize(vertexCount);
			wrapper.Subset.Skinned = true;
			wrapper.Subset.BoneId = -1;

			for (u32 j = 0; j < boneCount; j++)
			{
				aiBone* bone = mesh->mBones[j];
				u32 boneIndex = getBoneIndexByName(bones, bone->mName.C_Str());
				wrapper.SubsetBones[j].BoneIndex = boneIndex;
				wrapper.SubsetBones[j].OffsetTransform = convertFromAiMatrix(bone->mOffsetMatrix);

				for (u32 k = 0; k < bone->mNumWeights; k++)
				{
					u32 v = bone->mWeights[k].mVertexId;
					f32 weight = bone->mWeights[k].mWeight;
					SVertexSkinInfo skinInfo;
					skinInfo.BoneId = j;
					skinInfo.Weight = weight;
					wrapper.VertexSkinInfos[v].push_back(skinInfo);
				}
			}
		}
		else // 如果该子集没有骨骼动画
		{
			wrapper.Subset.Skinned = false;
			wrapper.Subset.BoneId = getBoneIndexBySubsetId(bones, i);
		}
	}
}


s32 getBoneIndexByName(const std::vector<SModelBoneWrapper>& bones, const char* name)
{
	for (u32 i = 0; i < bones.size(); i++)
	{
		if (strcmp(bones[i].Bone.Name, name) == 0)
			return i;
	}
	return -1;
}

s32 getBoneIndexBySubsetId(const std::vector<SModelBoneWrapper>& bones, u32 id)
{
	for (u32 i = 0; i < bones.size(); i++)
	{
		if (bones[i].SubsetId == id)
			return i;
	}
	return -1;
}

void loadAnimations(SModelFileHeader& header,
	std::vector<SBoneAnimationClip>& animationClips,
	const std::vector<SModelBoneWrapper>& bones,
	const aiScene* scene)
{
	header.AnimationClipCount = scene->mNumAnimations;
	animationClips.resize(header.AnimationClipCount);

	for (u32 i = 0; i < header.AnimationClipCount; i++)
	{
		SBoneAnimationClip& anim_clip = animationClips[i];
		aiAnimation* anim = scene->mAnimations[i];

		strcpy_s(anim_clip.Name, anim->mName.C_Str());

		for (u32 j = 0; j < anim->mNumChannels; j++)
		{
			const aiNodeAnim* node_anim = anim->mChannels[j];
			u32 boneIndex = getBoneIndexByName(bones, node_anim->mNodeName.C_Str());

			SBoneAnimation boneAnimation;
			boneAnimation.BoneID = boneIndex;

			for (u32 k = 0; k < node_anim->mNumPositionKeys; k++)
			{
				STranslationAnimateFrame frame;
				frame.TimePos = (f32)node_anim->mPositionKeys[k].mTime;
				memcpy(&frame.Translation, &node_anim->mPositionKeys[k].mValue, sizeof(XMFLOAT3));
				boneAnimation.TranslationFrames.push_back(frame);
			}

			for (u32 k = 0; k < node_anim->mNumScalingKeys; k++)
			{
				SScaleAnimateFrame frame;
				frame.TimePos = (f32)node_anim->mScalingKeys[k].mTime;
				memcpy(&frame.Scale, &node_anim->mScalingKeys[k].mValue, sizeof(XMFLOAT3));
				boneAnimation.ScaleFrames.push_back(frame);
			}

			for (u32 k = 0; k < node_anim->mNumRotationKeys; k++)
			{
				SRotationAnimateFrame rotateFrame;
				rotateFrame.TimePos = (f32)node_anim->mRotationKeys[k].mTime;
				aiQuaternion quat = node_anim->mRotationKeys[k].mValue;
				rotateFrame.RotationQuat.x = quat.x;
				rotateFrame.RotationQuat.y = quat.y;
				rotateFrame.RotationQuat.z = quat.z;
				rotateFrame.RotationQuat.w = quat.w;
				boneAnimation.RotationFrames.push_back(rotateFrame);
			}

			anim_clip.BoneAnimations.push_back(boneAnimation);
		}
	}
}
