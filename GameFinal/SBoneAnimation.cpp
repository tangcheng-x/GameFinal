#include "stdafx.h"
#include "IAnimatedMesh.h"

f32 SBoneAnimationClip::GetClipStartTime() const
{
	f32 t = FLT_MAX;
	for (u32 i = 0; i < BoneAnimations.size(); i++)
	{
		t = min(t, BoneAnimations[i].GetStartTime());
	}
	return t;
}

f32 SBoneAnimationClip::GetClipEndTime() const
{
	f32 t = 0.0f;
	for (u32 i = 0; i < BoneAnimations.size(); i++)
	{
		t = max(t, BoneAnimations[i].GetEndTime());
	}
	return t;
}

