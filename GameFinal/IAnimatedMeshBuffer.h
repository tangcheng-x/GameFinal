#ifndef __SKIN_MODEL_RENDER_BUFFER_INTERFACE_H__
#define __SKIN_MODEL_RENDER_BUFFER_INTERFACE_H__

#include "IReferenceCounted.h"

class IAnimatedMeshBuffer : public IReferenceCounted
{
public:
	virtual void bindIndexBuffer() = 0;

	virtual void bindVertexBuffer(bool bAnimated) = 0;

	virtual void drawIndexed(u32 start, u32 count, u32 baseVertex) = 0;
};

#endif
