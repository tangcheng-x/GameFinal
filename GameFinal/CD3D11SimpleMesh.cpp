#include "stdafx.h"
#include "CD3D11SimpleMesh.h"

/*
bool CD3D11SimpleMesh::init(void* vertices,
	void* indices,
	u32 vertexCount,
	u32 indicesCount,
	u32 vertexStride)
{
	HRESULT hr;

	mVertexCount = vertexCount;
	mIndiceCount = indicesCount;
	mVertexStride = vertexStride;

	if (vertices == nullptr)
	{
		GF_PRINT_CONSOLE_INFO("Create D3D11 Mesh! Because the vertices parameters is empty!");
		return false;
	}

	if (indices == nullptr || indicesCount == 0)
	{
		mUseIndexBuffer = false;
	}

	// TODO: should modify in the future
	if (mMemoryUsage != EMU_STATIC && mMemoryUsage != EMU_DEFAULT)
	{
		throw std::runtime_error("can not create dynamic now!");
		return false;
	}

	D3D11_USAGE d3dUsage = getD3dUsage(mMemoryUsage);
	D3D11_BUFFER_DESC vbDesc;
	vbDesc.ByteWidth = vertexStride * vertexCount;
	vbDesc.Usage = d3dUsage;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.StructureByteStride = 0;
	vbDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = vertices;
	hr = md3dDevice->CreateBuffer(&vbDesc, &vbData, &md3dVertexBuffer);

	if (FAILED(hr))
		return false;

	if (mUseIndexBuffer)
	{
		u32 indiceStride = sizeof(WORD);
		if (mMeshAttribute & EMA_32BIT_INDEX)
			indiceStride = sizeof(DWORD);

		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = indiceStride * indicesCount;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;
		ibDesc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA ibData;
		ibData.pSysMem = indices;

		hr = md3dDevice->CreateBuffer(&ibDesc, &ibData, &md3dIndexBuffer);

		if (FAILED(hr))
			return false;
	}

	return true;
}

void CD3D11SimpleMesh::bind()
{
	u32 offset = 0;

	CD3D11Driver::SD3D11DriverState& currentState = md3d11Driver->D3D11DriverState;
	if (currentState.VertexBuffers[0] != md3dVertexBuffer)
	{
		md3dDeviceContext->IASetVertexBuffers(0, 1, &md3dVertexBuffer, &mVertexStride, &offset);
		currentState.VertexBuffers[0] = md3dVertexBuffer;
	}

	if (mUseIndexBuffer && currentState.IndexBuffer != md3dIndexBuffer)
	{
		md3dDeviceContext->IASetIndexBuffer(md3dIndexBuffer, mIndexFormat, 0);
		currentState.IndexBuffer = md3dIndexBuffer;
	}
}

void CD3D11SimpleMesh::drawSubset(u32 index)
{
	if (index > 0)
	{
		GF_PRINT_CONSOLE_INFO("Simple Mesh only have one subset!\n");
	}

	if (mUseIndexBuffer)
	{
		md3dDeviceContext->DrawIndexed(mIndiceCount, 0, 0);
	}
	else
	{
		md3dDeviceContext->Draw(mVertexCount, 0);
	}
}

void CD3D11SimpleMesh::drawIndexed(u32 start, u32 count, u32 baseVertex)
{
	if (!mUseIndexBuffer)
	{
		GF_PRINT_CONSOLE_INFO("The mesh don't use index buffer.\n");
		return;
	}

	md3dDeviceContext->DrawIndexed(count, start, baseVertex);
}

void CD3D11SimpleMesh::draw(u32 start, u32 count)
{
	md3dDeviceContext->Draw(count, start);
}
*/