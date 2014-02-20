#ifndef __MODEL_MESH_INTERFACE_H__
#define __MODEL_MESH_INTERFACE_H__

#include "IMesh.h"
#include "IMaterial.h"

class IModelMesh : public IMesh
{
public:
	struct SModelSubset
	{
		u32					IndexCount;
		u32					StartIndexLocation;
		u32					BaseVertexLocation;
		IMaterial*			Material;
	};

public:
	IModelMesh(const std::string& name,
		u32 sortcode)
		:IMesh(name, sortcode)
	{

	}

	virtual u32 getSubsetCount() const = 0;

	virtual IMaterial* getMaterial(u32 index) const = 0;

	virtual bool getMaterialName(u32 index, std::string& name) const = 0;

	virtual void drawSubset(u32 index) const = 0;

	virtual void bind() = 0;

protected:
	

};


#endif

