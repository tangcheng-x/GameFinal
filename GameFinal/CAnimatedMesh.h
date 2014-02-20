#ifndef __ANIMATED_MODEL_MESH_CLASS_H__
#define __ANIMATED_MODEL_MESH_CLASS_H__

#include "IAnimatedMesh.h"
#include "IAnimatedMeshBuffer.h"

class CAnimatedMesh : public IAnimatedMesh
{
public:
	CAnimatedMesh(const std::string& name,
		u32 sortcode,
		
		const std::vector<SAnimatedModelSubset>& subsets,
		const std::vector<SModelBone>& bones,
		const std::vector<SBoneAnimationClip>& animateClips,
		IAnimatedMeshBuffer* renderBuffer)
		:IAnimatedMesh(name, sortcode)
		, mRenderBuffer(renderBuffer)
		, mSubsets(subsets)
		, mBonesTree(bones)
		, mAnimationClips(animateClips)
	{

	}

	virtual u32 getSubsetCount() const
	{
		return mSubsets.size();
	}

	virtual IMaterial* getMaterial(u32 index) const
	{
		if (index >= mSubsets.size())
			return nullptr;

		return mSubsets[index].Material;
	}

	virtual bool getMaterialName(u32 index, std::string& name) const
	{
		if (index >= mSubsets.size())
			return false;

		IMaterial* material = mSubsets[index].Material;
		if (!material)
			return false;

		name = material->getName();
		return true;
	}

	virtual void getRelativeBoneTransforms(std::vector<XMFLOAT4X4>& transforms) const;

	virtual void getRelativeBoneTransforms(u32 animationId, f32& timePos, std::vector<XMFLOAT4X4>& transforms) const;

	virtual void getAbsoluteBoneTransforms(std::vector<XMFLOAT4X4>& transforms) const;

	virtual void getAbsoluteBoneTransforms(const std::vector<XMFLOAT4X4>& relatives, std::vector<XMFLOAT4X4>& transforms) const;

	virtual const std::vector<SAnimatedModelSubset>& getSubsets() const
	{
		return mSubsets;
	}

	virtual u32 getBoneCount() const
	{
		return mBonesTree.size();
	}

	virtual void bind()
	{
		mRenderBuffer->bindIndexBuffer();
	}

	virtual void drawSubset(u32 index) const
	{
		if (index >= mSubsets.size())
			return;

		const SAnimatedModelSubset& subset = mSubsets[index];
		if (subset.Skinned)
			mRenderBuffer->bindVertexBuffer(true);
		else
			mRenderBuffer->bindVertexBuffer(false);

		mRenderBuffer->drawIndexed(subset.StartIndexLocation, subset.IndexCount, subset.BaseVertexLocation);
	}

	virtual E_MESH_TYPE getType() const
	{
		return EMT_ANIMATE_MODEL_MESH;
	}

	virtual u32 getAnimationCount() const;

	virtual bool getAnimationName(u32 id, std::string& name) const;

	virtual s32 getAnimationId(const std::string& name) const;

private:
	
	IAnimatedMeshBuffer*						mRenderBuffer;
	std::vector<SAnimatedModelSubset>			mSubsets;
	std::vector<SModelBone>						mBonesTree;		/* ¹Ç÷ÀÊ÷ */
	std::vector<SBoneAnimationClip>				mAnimationClips;

};


#endif
