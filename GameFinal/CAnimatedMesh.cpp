#include "stdafx.h"
#include "CAnimatedMesh.h"



void CAnimatedMesh::getAbsoluteBoneTransforms(const std::vector<XMFLOAT4X4>& relatives, std::vector<XMFLOAT4X4>& transforms) const
{
	for (u32 i = 0; i < mBonesTree.size(); i++)
	{
		const SModelBone& bone = mBonesTree[i];
		if (bone.Parent == -1)
		{
			transforms[i] = relatives[i];
		}
		else
		{
			XMMATRIX parentToRoot = XMLoadFloat4x4(&transforms[bone.Parent]);
			XMMATRIX relativeToParent = XMLoadFloat4x4(&relatives[i]);

			XMMATRIX combinedMatrix = relativeToParent * parentToRoot;
			XMStoreFloat4x4(&transforms[i], combinedMatrix);
		}
	}
}

void CAnimatedMesh::getAbsoluteBoneTransforms(std::vector<XMFLOAT4X4>& transforms) const
{
	for (u32 i = 0; i < mBonesTree.size(); i++)
	{
		const SModelBone& bone = mBonesTree[i];
		if (bone.Parent == -1)
		{
			transforms[i] = bone.Transform;
		}
		else
		{
			XMMATRIX parentToRoot = XMLoadFloat4x4(&transforms[bone.Parent]);
			XMMATRIX relativeToParent = XMLoadFloat4x4(&bone.Transform);

			XMMATRIX combinedMatrix = relativeToParent * parentToRoot;
			XMStoreFloat4x4(&transforms[i], combinedMatrix);
		}
	}
}

u32 CAnimatedMesh::getAnimationCount() const
{
	return mAnimationClips.size();
}

bool CAnimatedMesh::getAnimationName(u32 id, std::string& name) const
{
	if (id >= mAnimationClips.size())
		return false;

	name = mAnimationClips[id].Name;
	return true;
}

s32 CAnimatedMesh::getAnimationId(const std::string& name) const
{
	for (u32 i = 0; i < mAnimationClips.size(); i++)
	{
		if (_stricmp(name.c_str(), mAnimationClips[i].Name) == 0)
			return i;
	}
	return -1;
}

void CAnimatedMesh::getRelativeBoneTransforms(std::vector<XMFLOAT4X4>& transforms) const
{
	for (u32 i = 0; i < mBonesTree.size(); i++)
	{
		transforms[i] = mBonesTree[i].Transform;
	}
}

void CAnimatedMesh::getRelativeBoneTransforms(u32 animationId, f32& t, std::vector<XMFLOAT4X4>& transforms) const
{
	if (animationId >= mAnimationClips.size())
		return;

	const SBoneAnimationClip& animClip = mAnimationClips[animationId];
	const std::vector<SBoneAnimation>& boneAnims = animClip.BoneAnimations;

	if (t > animClip.GetClipEndTime())
		t = 0.0f;


	for (u32 i = 0; i < boneAnims.size(); i++)
	{
		int boneID = boneAnims[i].BoneID;
		boneAnims[i].Interpolate(t, transforms[boneID]);
	}
}

