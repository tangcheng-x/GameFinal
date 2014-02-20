#ifndef __CMESHNODE_H__
#define __CMESHNODE_H__

#include "IMeshNode.h"
#include "ISimpleMesh.h"

class CMeshNode : public IMeshNode
{
public:
	CMeshNode(ISceneNode* parent,
		ISceneManager* smgr,
		ISimpleMesh* mesh,
		IMaterial* material = nullptr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f))
		:IMeshNode(parent, smgr, position, rotation, scale)
		, mMesh(mesh)
		, mMaterial(material)
	{
		AddReferenceCounted(mMaterial);
	}

	virtual ~CMeshNode()
	{
		ReleaseReferenceCounted(mMesh);
		ReleaseReferenceCounted(mMaterial);
	}


	virtual void render();

	virtual void OnRegisterSceneNode();

	virtual bool setMaterialName(const std::string& name, u32 subset = 0);

	virtual void calcSortCode();

	virtual bool setMaterial(IMaterial* material, u32 subset = 0)
	{
		ReleaseReferenceCounted(mMaterial);
		mMaterial = material;
		AddReferenceCounted(mMaterial);
		return true;
	}

	virtual IMaterial* getMaterial(u32 subset = 0)
	{
		return mMaterial;
	}

	virtual u32 getSubsetCount() const
	{
		return 1;
	}

private:
	ISimpleMesh*			mMesh;
	IMaterial*				mMaterial;
};

#endif