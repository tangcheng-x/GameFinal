#ifndef __MESH_BUFFER_INTERFACE_H__
#define __MESH_BUFFER_INTERFACE_H__

#include "IReferenceCounted.h"
#include "IMesh.h"

/*
enum E_MESH_ATTRIBUTE
{
	EMA_USE_INDEX_BUFFER = (1 << 0),
	EMA_32BIT_INDEX = (1 << 1),
	EMA_DEFAULT = EMA_USE_INDEX_BUFFER
};
*/



class IMeshBuffer : public IReferenceCounted
{
public:
	IMeshBuffer(E_MEMORY_USAGE usage)
		: mMemoryUsage(usage)
	{

	}

	virtual bool init(void* vertices,
		void* indices,
		u32 vertexCount,
		u32 indicesCount,
		u32 vertexStride, 
		bool bit32Index) = 0;

	virtual void bind() = 0;

	virtual void draw() const = 0;

	virtual void drawIndexed(u32 start, u32 count, u32 baseVertex) const = 0;

	virtual void draw(u32 start, u32 count) const = 0;

	

protected:
	E_MEMORY_USAGE		mMemoryUsage;
};

#endif