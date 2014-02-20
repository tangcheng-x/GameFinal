#ifndef __SIMPLE_MESH_INTERFACE_H__
#define __SIMPLE_MESH_INTERFACE_H__

#include "IMesh.h"
#include "IMaterial.h"

class ISimpleMesh : public IMesh
{
public:
	ISimpleMesh(const std::string& name,
		u32 sortcode)
		:IMesh(name, sortcode)
	{

	}
	
	virtual void bind() = 0;

	virtual void draw() const = 0;

};


#endif
