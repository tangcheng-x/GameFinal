#ifndef __SIMPLE_MESH_CLASS_H__
#define __SIMPLE_MESH_CLASS_H__

#include "ISimpleMesh.h"
#include "IMaterialManager.h"

class CSimpleMesh : public ISimpleMesh
{
public:
	CSimpleMesh(const std::string& name,
		u32 sortcode,
		IMeshBuffer* buffer)
		:ISimpleMesh(name, sortcode)
		, mMeshBuffer(buffer)
	{

	}

	virtual void bind();

	virtual void draw() const;

	virtual E_MESH_TYPE getType() const
	{
		return EMT_SIMPLE_MESH;
	}

private:
	IMeshBuffer*			mMeshBuffer;

};

#endif
