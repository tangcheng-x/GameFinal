#ifndef __MODEL_MESH_NODE_CLASS_H__
#define __MODEL_MESH_NODE_CLASS_H__

#include "IMeshNode.h"
#include "IModelMesh.h"

class CModelMeshNode : public IMeshNode
{
public:
	CModelMeshNode(ISceneNode* parent,
		ISceneManager* smgr,
		IModelMesh* mesh,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f))
		:IMeshNode(parent, smgr, position, rotation, scale)
		, mMesh(mesh)
		, mMaterials(mesh->getSubsetCount())
	{
		for (u32 i = 0; i < mesh->getSubsetCount(); i++)
		{
			mMaterials[i] = mesh->getMaterial(i);
			AddReferenceCounted(mMaterials[i]);
		}
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

private:
	
	IModelMesh*						mMesh;
	
	std::vector<IMaterial*>			mMaterials;
};


#endif

