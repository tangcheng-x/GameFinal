#include "stdafx.h"
#include "CD3D11AnimatedMeshBuffer.h"
#include "CD3D11SimpleMesh.h"


bool CD3D11AnimatedMeshBuffer::init(void* vertices,
	void* animateVertices,
	void* indices,
	u32 vertexCount,
	u32 animateVertexCount,
	u32 indicesCount,
	u32 vertexStride,
	u32 animateVertexStride,
	bool bit32Index)
{
	HRESULT hr;

	mVertexCount = vertexCount;
	mAnimateVertexCount = animateVertexCount;
	mIndiceCount = indicesCount;
	mVertexStride = vertexStride;
	mAnimateVertexStride = animateVertexStride;

	/*
	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT3 Weights;
		u8		 BoneIndices[4];
	};

	std::ofstream file;
	file.open("log3.txt", std::ios::out);

	Vertex* p = (Vertex*)animateVertices;
	for (u32 i = animateVertexCount - 1; i > 0; i--)
	{
		Vertex v = p[i];
		file << v.Pos.x << " " << v.Pos.y << " " << v.Pos.z << "\n";
		file << (u32)v.BoneIndices[0] << " " << (u32)v.BoneIndices[1] << " " << (u32)v.BoneIndices[2] << " " << (u32)v.BoneIndices[3] << "\n";
		file << v.Weights.x << " " << v.Weights.y << " " << v.Weights.z << "\n";
	}
	
	file.close();
	*/


	if (vertices == nullptr && animateVertices == nullptr)
	{
		GF_PRINT_CONSOLE_INFO("Create D3D11 Mesh failed! Because the vertices parameters is empty!\n");
		return false;
	}

	if (bit32Index)
		mIndexFormat = DXGI_FORMAT_R32_UINT;
	else
		mIndexFormat = DXGI_FORMAT_R16_UINT;

	// TODO: should modify in the future
	if (mMemoryUsage != EMU_STATIC && mMemoryUsage != EMU_DEFAULT)
	{
		throw std::runtime_error("can not create dynamic now!");
		return false;
	}
	
	D3D11_USAGE d3dUsage = getD3d11Usage(mMemoryUsage);
	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = d3dUsage;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.StructureByteStride = 0;
	vbDesc.MiscFlags = 0;

	/* create vertex buffer */
	if (mVertexCount > 0)
	{
		vbDesc.ByteWidth = vertexStride * vertexCount;
		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = vertices;
		hr = md3dDevice->CreateBuffer(&vbDesc, &vbData, &md3dVertexBuffer);
		if (FAILED(hr))
			return false;
	}

	/* create animate vertex buffer. */
	if (mAnimateVertexCount > 0)
	{
		vbDesc.ByteWidth = animateVertexStride * animateVertexCount;
		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = animateVertices;
		hr = md3dDevice->CreateBuffer(&vbDesc, &vbData, &md3dAnimateVertexBuffer);
		if (FAILED(hr))
			return false;
	}
	
	u32 indiceStride = sizeof(WORD);
	if (bit32Index)
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

	return true;
}

void CD3D11AnimatedMeshBuffer::bindIndexBuffer()
{
	md3dDeviceContext->IASetIndexBuffer(md3dIndexBuffer, mIndexFormat, 0);
	
	CD3D11Driver::SD3D11DriverState& currentState = md3d11Driver->D3D11DriverState;
	if (currentState.IndexBuffer != md3dIndexBuffer)
	{
		md3dDeviceContext->IASetIndexBuffer(md3dIndexBuffer, mIndexFormat, 0);
		currentState.IndexBuffer = md3dIndexBuffer;
	}
	
}

void CD3D11AnimatedMeshBuffer::bindVertexBuffer(bool bAnimated)
{
	CD3D11Driver::SD3D11DriverState& currentState = md3d11Driver->D3D11DriverState;
	if (bAnimated)
	{
		if (currentState.VertexBuffers[0] != md3dAnimateVertexBuffer)
		{
			u32 offset = 0;
			md3dDeviceContext->IASetVertexBuffers(0, 1, &md3dAnimateVertexBuffer, &mAnimateVertexStride, &offset);
			currentState.VertexBuffers[0] = md3dAnimateVertexBuffer;
		}
	}
	else
	{
		if (currentState.VertexBuffers[0] != md3dVertexBuffer)
		{
			u32 offset = 0;
			md3dDeviceContext->IASetVertexBuffers(0, 1, &md3dVertexBuffer, &mVertexStride, &offset);
			currentState.VertexBuffers[0] = md3dVertexBuffer;
		}
	}
}

void CD3D11AnimatedMeshBuffer::drawIndexed(u32 start, u32 count, u32 baseVertex)
{
	md3dDeviceContext->DrawIndexed(count, start, baseVertex);
}
