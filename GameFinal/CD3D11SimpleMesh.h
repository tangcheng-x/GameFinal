#ifndef __CD3D11SIMPLEMESH_H__
#define __CD3D11SIMPLEMESH_H__

#include "IMesh.h"
#include "gfUtil.h"
#include "D3DUtil.h"
#include "CD3D11Driver.h"

/*

class CD3D11SimpleMesh : public IMesh
{
public:
	CD3D11SimpleMesh(
		const std::string& name,
		u32 sortcode,
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pd3dDeviceContext,
		CD3D11Driver* pd3d11Driver,
		u32 attr = EMA_DEFAULT,
		E_MEMORY_USAGE usage = EMU_STATIC)
		:IMesh(name, sortcode, attr, usage)
		, md3dDevice(pd3dDevice)
		, md3dDeviceContext(pd3dDeviceContext)
		, md3d11Driver(pd3d11Driver)
		, md3dVertexBuffer(nullptr)
		, md3dIndexBuffer(nullptr)
		, mUseIndexBuffer(false)
	{
		if (attr & EMA_USE_INDEX_BUFFER)
			mUseIndexBuffer = true;

		if (attr & EMA_32BIT_INDEX)
			mIndexFormat = DXGI_FORMAT_R32_UINT;
		else
			mIndexFormat = DXGI_FORMAT_R16_UINT;
	}

	bool init(void* vertices,
		void* indices,
		u32 vertexCount,
		u32 indicesCount,
		u32 vertexStride);

	virtual void bind();

	virtual void drawSubset(u32 index);

	virtual void drawIndexed(u32 start, u32 count, u32 baseVertex);

	virtual void draw(u32 start, u32 count);

	virtual ~CD3D11SimpleMesh()
	{
		ReleaseCOM(md3dVertexBuffer);
		ReleaseCOM(md3dIndexBuffer);
	}

	static D3D11_USAGE getD3dUsage(E_MEMORY_USAGE usage)
	{
		switch (usage)
		{
		case EMU_STATIC:	return D3D11_USAGE_IMMUTABLE;
		case EMU_DEFAULT:	return D3D11_USAGE_DEFAULT;
		case EMU_DYNAMIC:	return D3D11_USAGE_DYNAMIC;
		case EMU_STAGING:	return D3D11_USAGE_STAGING;
		}
		return D3D11_USAGE_IMMUTABLE;
	}

	virtual E_MESH_TYPE getType() const
	{
		return EMT_SIMPLE_MESH;
	}

protected:
	ID3D11Device*			md3dDevice;
	ID3D11DeviceContext*	md3dDeviceContext;

	CD3D11Driver*			md3d11Driver;

	ID3D11Buffer*			md3dVertexBuffer;
	ID3D11Buffer*			md3dIndexBuffer;

	u32						mVertexCount;
	u32						mIndiceCount;
	u32						mVertexStride;

	bool					mUseIndexBuffer;

	DXGI_FORMAT				mIndexFormat;
};
*/

#endif

