#ifndef __ANIMATED_MESH_NODE_CLASS_H__
#define __ANIMATED_MESH_NODE_CLASS_H__

#include "IAnimatedMeshNode.h"
#include "IAnimatedMesh.h"

class CAnimatedMeshNode : public IAnimatedMeshNode
{
public:
	CAnimatedMeshNode(ISceneNode* parent,
		ISceneManager* smgr,
		IAnimatedMesh* mesh,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f))
		:IAnimatedMeshNode(parent, smgr, position, rotation, scale)
		, mMesh(mesh)
		, mAbsoluteBoneTransforms(mesh->getBoneCount())
		, mRelativeBoneTransforms(mesh->getBoneCount())
		, mMaterials(mesh->getSubsetCount())
	{
		u32 maxBoneCountInSubset = 0;
		const std::vector<SAnimatedModelSubset>& subsets = mMesh->getSubsets();

		for (u32 i = 0; i < subsets.size(); i++)
		{
			if (subsets[i].Skinned && subsets[i].Bones.size() > maxBoneCountInSubset)
				maxBoneCountInSubset = subsets[i].Bones.size();
		}

		mSubsetBoneTransforms.resize(maxBoneCountInSubset);

		for (u32 i = 0; i < subsets.size(); i++)
		{
			mMaterials[i] = subsets[i].Material;
		}

		mMesh->getRelativeBoneTransforms(mRelativeBoneTransforms);
		mTimePos = 0;
		mCurrAnimationId = 0;
	}

	virtual bool setMaterial(IMaterial* material, u32 subset = 0);

	virtual IMaterial* getMaterial(u32 subset = 0);

	virtual bool setMaterialName(const std::string& name, u32 subset = 0);

	virtual void render();

	virtual void OnRegisterSceneNode();

	virtual void calcSortCode();

	virtual u32 getSubsetCount() const
	{
		return mMesh->getSubsetCount();
	}

	virtual bool setAnimation(u32 id);

	virtual bool setAnimation(const std::string& name);

	virtual void addTime(f32 t);

	void updateAbsoluteBoneTransforms();

private:
	void updateSubsetBones(const std::vector<SModelSubsetBone>& bones);

	IAnimatedMesh*						mMesh;
	std::vector<XMFLOAT4X4>				mSubsetBoneTransforms;
	std::vector<XMFLOAT4X4>				mAbsoluteBoneTransforms;
	std::vector<XMFLOAT4X4>				mRelativeBoneTransforms;
	std::vector<IMaterial*>				mMaterials;

	f32									mTimePos;
	u32									mCurrAnimationId;
};

#endif
