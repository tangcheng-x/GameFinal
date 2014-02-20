#ifndef __ANIMATED_MODEL_MESH_INTERFACE_H__
#define __ANIMATED_MODEL_MESH_INTERFACE_H__

#include "IModelMesh.h"

struct SModelSubsetBone
{
	u32							BoneIndex;
	XMFLOAT4X4					OffsetTransform;
};

struct SModelBone
{
	char						Name[32];
	s32							Parent;
	XMFLOAT4X4					Transform;
};

struct STranslationAnimateFrame
{
	f32				TimePos;
	XMFLOAT3		Translation;

	bool operator< (const STranslationAnimateFrame& frame) const
	{
		return TimePos < frame.TimePos;
	}

	bool operator< (f32 t) const
	{
		return TimePos < t;
	}
};

struct SScaleAnimateFrame
{
	f32				TimePos;
	XMFLOAT3		Scale;

	bool operator< (const STranslationAnimateFrame& frame) const
	{
		return TimePos < frame.TimePos;
	}

	bool operator< (f32 t) const
	{
		return TimePos < t;
	}
};

struct SRotationAnimateFrame
{
	f32				TimePos;
	XMFLOAT4		RotationQuat;
	
	bool operator< (const STranslationAnimateFrame& frame) const
	{
		return TimePos < frame.TimePos;
	}

	bool operator< (f32 t) const
	{
		return TimePos < t;
	}
};

struct SBoneAnimation
{
	u32											BoneID;
	std::vector<STranslationAnimateFrame>		TranslationFrames;
	std::vector<SScaleAnimateFrame>				ScaleFrames;
	std::vector<SRotationAnimateFrame>			RotationFrames;

	f32 GetStartTime() const
	{
		f32 t1 = TranslationFrames.front().TimePos;
		f32 t2 = ScaleFrames.front().TimePos;
		f32 t3 = RotationFrames.front().TimePos;

		//std::upper_bound(TranslationFrames.begin(), TranslationFrames.end(), 1.0f);

		return min(t1, min(t2, t3));
	}

	f32 GetEndTime() const
	{
		f32 t1 = TranslationFrames.back().TimePos;
		f32 t2 = ScaleFrames.back().TimePos;
		f32 t3 = RotationFrames.back().TimePos;

		return max(t1, max(t2, t3));
	}

	
	template<class T>
	static u32 GetLessEqualFrameTimeIndex(const std::vector<T>& frames, f32 t)
	{
		u32 left = 0;
		u32 right = frames.size() - 1;
		u32 mid;


		while (left + 1 < right)
		{
			mid = (left + right) >> 1;
			if (frames[mid].TimePos < t + 0.0000001f && frames[mid].TimePos > t - 0.0000001f)
				return mid;

			if (t < frames[mid].TimePos)
				right = mid - 1;
			else
				left = mid;
		}

		if (left == right)
			return left;

		if (frames[right].TimePos < t)
			return right;

		return left;
	}


	void Interpolate(float t, XMFLOAT4X4& M) const
	{
		XMVECTOR S;
		XMVECTOR T;
		XMVECTOR R;

		if (t <= TranslationFrames.front().TimePos)
			T = XMLoadFloat3(&TranslationFrames.front().Translation);
		else
		{
			u32 frameIndex = GetLessEqualFrameTimeIndex(TranslationFrames, t);
			if (frameIndex == TranslationFrames.size() - 1)
				T = XMLoadFloat3(&TranslationFrames.back().Translation);
			else
			{
				f32 lerpPercent = (t - TranslationFrames[frameIndex].TimePos)
					/ (TranslationFrames[frameIndex + 1].TimePos - TranslationFrames[frameIndex].TimePos);

				XMVECTOR p0 = XMLoadFloat3(&TranslationFrames[frameIndex].Translation);
				XMVECTOR p1 = XMLoadFloat3(&TranslationFrames[frameIndex + 1].Translation);
				T = XMVectorLerp(p0, p1, lerpPercent);
			}
		}

		//Scale
		if (t <= ScaleFrames.front().TimePos)
			S = XMLoadFloat3(&ScaleFrames.front().Scale);
		else
		{
			u32 frameIndex = GetLessEqualFrameTimeIndex(ScaleFrames, t);

			if (frameIndex == ScaleFrames.size() - 1)
				S = XMLoadFloat3(&ScaleFrames.back().Scale);
			else
			{
				f32 lerpPercent = (t - ScaleFrames[frameIndex].TimePos)
					/ (ScaleFrames[frameIndex + 1].TimePos - ScaleFrames[frameIndex].TimePos);

				XMVECTOR p0 = XMLoadFloat3(&ScaleFrames[frameIndex].Scale);
				XMVECTOR p1 = XMLoadFloat3(&ScaleFrames[frameIndex + 1].Scale);
				S = XMVectorLerp(p0, p1, lerpPercent);
			}
		}

		if (t <= RotationFrames.front().TimePos)
			R = XMLoadFloat4(&RotationFrames.front().RotationQuat);
		else
		{
			u32 frameIndex = GetLessEqualFrameTimeIndex(RotationFrames, t);
			if (frameIndex == RotationFrames.size() - 1)
				R = XMLoadFloat4(&RotationFrames.back().RotationQuat);
			else
			{
				f32 lerpPercent = (t - RotationFrames[frameIndex].TimePos)
					/ (RotationFrames[frameIndex + 1].TimePos - RotationFrames[frameIndex].TimePos);

				XMVECTOR q0 = XMLoadFloat4(&RotationFrames[frameIndex].RotationQuat);
				XMVECTOR q1 = XMLoadFloat4(&RotationFrames[frameIndex + 1].RotationQuat);
				R = XMQuaternionSlerp(q0, q1, lerpPercent);
			}
		}

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, R, T));
	}

};

struct SBoneAnimationClip
{
	char								Name[32];
	std::vector<SBoneAnimation>			BoneAnimations;

	f32 GetClipStartTime()const;
	f32 GetClipEndTime()const;

	f32 Interpolate(f32 t, std::vector<XMFLOAT4X4>& boneTransforms)const;
};

struct SAnimatedModelSubset
{
	u32								IndexCount;
	u32								StartIndexLocation;
	u32								BaseVertexLocation;
	s32								BoneId;
	bool							Skinned;
	IMaterial*						Material;
	std::vector<SModelSubsetBone>	Bones;
};

class IAnimatedMesh : public IModelMesh
{
public:
	IAnimatedMesh(const std::string& name,
		u32 sortcode)
		:IModelMesh(name, sortcode)
	{

	}

	virtual void getAbsoluteBoneTransforms(std::vector<XMFLOAT4X4>& transforms) const = 0;

	virtual void getAbsoluteBoneTransforms(const std::vector<XMFLOAT4X4>& relatives, std::vector<XMFLOAT4X4>& transforms) const = 0;

	virtual void getRelativeBoneTransforms(std::vector<XMFLOAT4X4>& transforms) const = 0;

	virtual void getRelativeBoneTransforms(u32 animationId, f32& timePos, std::vector<XMFLOAT4X4>& transforms) const = 0;

	virtual const std::vector<SAnimatedModelSubset>& getSubsets() const = 0;

	virtual u32 getBoneCount() const = 0;

	virtual u32 getAnimationCount() const = 0;

	virtual bool getAnimationName(u32 id, std::string& name) const = 0;

	virtual s32 getAnimationId(const std::string& name) const = 0;
	
	virtual u32 getSubsetCount() const = 0;

	virtual IMaterial* getMaterial(u32 index) const = 0;

	virtual bool getMaterialName(u32 index, std::string& name) const = 0;

	virtual void drawSubset(u32 index) const = 0;

	virtual void bind() = 0;
};

#endif
