#ifndef __CD3D11_ANIMATED_MESH_CLASS_H__
#define __CD3D11_ANIMATED_MESH_CLASS_H__

#include "IAnimatedMeshBuffer.h"
#include "CD3D11Driver.h"
#include "D3DUtil.h"

class CD3D11AnimatedMeshBuffer : public IAnimatedMeshBuffer
{
public:
	CD3D11AnimatedMeshBuffer(
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pd3dDeviceContext,
		CD3D11Driver* pd3d11Driver,
		E_MEMORY_USAGE usage)
		: md3dDevice(pd3dDevice)
		, md3dDeviceContext(pd3dDeviceContext)
		, md3d11Driver(pd3d11Driver)
		, mMemoryUsage(usage)
		, md3dVertexBuffer(nullptr)
		, md3dAnimateVertexBuffer(nullptr)
		, md3dIndexBuffer(nullptr)
	{
		
	}

	bool init(void* vertices,
		void* animateVertices,
		void* indices,
		u32 vertexCount,
		u32 animateVertexCount,
		u32 indicesCount,
		u32 vertexStride,
		u32 animateVertexStride,
		bool bit32Index);

	void bindIndexBuffer();

	void bindVertexBuffer(bool bAnimated);

	void drawIndexed(u32 start, u32 count, u32 baseVertex);

	virtual ~CD3D11AnimatedMeshBuffer()
	{
		ReleaseCOM(md3dVertexBuffer);
		ReleaseCOM(md3dAnimateVertexBuffer);
		ReleaseCOM(md3dIndexBuffer);
	}

	virtual E_MESH_TYPE getType() const
	{
		return EMT_ANIMATE_MODEL_MESH;
	}

private:
	ID3D11Device*			md3dDevice;
	ID3D11DeviceContext*	md3dDeviceContext;

	CD3D11Driver*			md3d11Driver;

	ID3D11Buffer*			md3dVertexBuffer;
	ID3D11Buffer*			md3dAnimateVertexBuffer;
	ID3D11Buffer*			md3dIndexBuffer;

	u32						mVertexCount;
	u32						mAnimateVertexCount;
	u32						mIndiceCount;
	u32						mVertexStride;
	u32						mAnimateVertexStride;
	DXGI_FORMAT				mIndexFormat;

	E_MEMORY_USAGE			mMemoryUsage;

};



#endif