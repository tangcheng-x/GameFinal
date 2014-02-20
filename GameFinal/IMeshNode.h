#ifndef __IMESHNODE_H__
#define __IMESHNODE_H__

#include "IMesh.h"
#include "ISceneNode.h"
#include "IPipeline.h"
#include "IMaterial.h"

class IMeshNode : public ISceneNode
{
public:
	IMeshNode(ISceneNode* parent,
		ISceneManager* smgr,
		const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
		const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f))
		:ISceneNode(parent, smgr, position, rotation, scale)
	{
		
	}

	virtual bool setMaterial(IMaterial* material, u32 subset = 0) = 0;

	virtual IMaterial* getMaterial(u32 subset = 0) = 0;

	virtual bool setMaterialName(const std::string& name, u32 subset = 0) = 0;

	virtual void render() = 0;

	virtual u32 getSubsetCount() const = 0;

};

#endif