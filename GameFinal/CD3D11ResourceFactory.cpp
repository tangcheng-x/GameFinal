#include "stdafx.h"
#include "CD3D11ResourceFactory.h"
#include "CD3D11Texture.h"
#include "CD3D11RenderState.h"
#include "CD3D11Pipeline.h"
#include "CD3D11Driver.h"
#include "CD3D11VertexShader.h"
#include "CD3D11PixelShader.h"
#include "CD3D11InputLayout.h"
#include "CD3D11MeshBuffer.h"
#include "CD3D11AnimatedMeshBuffer.h"
#include "gfUtil.h"

CD3D11ResourceFactory::CD3D11ResourceFactory(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pd3dDeviceContext,
	CD3D11Driver* d3dDriver)
	:md3dDevice(pd3dDevice)
	, md3dDeviceContext(pd3dDeviceContext)
	, md3dDriver(d3dDriver)
{
	md3dRenderStateCache = new CD3D11RenderStateCache(pd3dDevice);
}


CD3D11ResourceFactory::~CD3D11ResourceFactory()
{
	md3dRenderStateCache->drop();
}


ITexture* CD3D11ResourceFactory::loadTextureFromFile(const std::string& name, const std::string& filename, u32 sortcode)
{
	CD3D11Texture* pTexture = new CD3D11Texture(md3dDevice, md3dDeviceContext, name, sortcode);
	
	if (!pTexture->loadFromFile(filename))
	{
		pTexture->drop();
		pTexture = nullptr;
	}

	return pTexture;
}

IRenderState* CD3D11ResourceFactory::createRenderState(const std::string& name)
{
	CD3D11RenderState* pRenderState = new CD3D11RenderState(name, md3dRenderStateCache);
	return pRenderState;
}

IPipeline* CD3D11ResourceFactory::createPipeline(const std::string& name,
	IShader** shaders,
	u32 shaderCount,
	IInputLayout* inputlayout,
	E_PRIMITIVE_TYPE primitiveType,
	IRenderState* renderState)
{
	CD3D11RenderState* pd3d11RenderState = dynamic_cast<CD3D11RenderState*>(renderState);

	CD3D11Pipeline* pipeline = new CD3D11Pipeline(name, shaders, 
		shaderCount, inputlayout, primitiveType, 
		pd3d11RenderState, md3dDeviceContext, 
		md3dDriver);

	return pipeline;
}

IShader* CD3D11ResourceFactory::createShaderFromFile(
	E_SHADER_TYPE shaderType,
	const char* szFilename,
	const char* szFunctionName,
	u32 id,
	const std::string& shaderName)
{
	CD3D11Shader* shader = nullptr;

	switch (shaderType)
	{
	case EST_VERTEX_SHADER:
		shader = new CD3D11VertexShader(id, shaderName, md3dDevice, md3dDeviceContext, md3dDriver);
		break;
	case EST_PIXEL_SHADER:
		shader = new CD3D11PixelShader(id, shaderName, md3dDevice, md3dDeviceContext, md3dDriver);
		break;
	}

	if (shader && !shader->loadFromFile(szFilename, szFunctionName))
	{
		shader->drop();
		shader = nullptr;
	}

	return shader;
}

IInputLayout* CD3D11ResourceFactory::createInputLayout(
	u32 sortCode,
	const std::vector<SInputLayoutElement>& elements,
	IShader* shader,
	u32 hashcode)
{
	if (shader->getType() != EST_VERTEX_SHADER)
	{
		GF_PRINT_CONSOLE_INFO("InputLayout must be created according to a vertex shader.\n");
		return nullptr;
	}

	CD3D11Shader* pdxShader = dynamic_cast<CD3D11Shader*>(shader);
	ID3D10Blob* pd3dBlob = pdxShader->getShaderBuffer();

	IInputLayout* layout = new CD3D11InputLayout(sortCode, md3dDevice, md3dDeviceContext, pd3dBlob,
		elements, hashcode);

	return layout;
}

IAnimatedMeshBuffer* CD3D11ResourceFactory::createAnimatedMeshBuffer(
	E_MEMORY_USAGE usage,
	void* vertices,
	void* animateVertices,
	void* indices,
	u32 vertexCount,
	u32 animateVertexCount,
	u32 indicesCount,
	u32 vertexStride,
	u32 animateVertexStride,
	bool bit32Index)
{

	CD3D11AnimatedMeshBuffer* buffer = new CD3D11AnimatedMeshBuffer(md3dDevice, md3dDeviceContext,
		md3dDriver, usage); 

	if (!buffer->init(vertices, animateVertices, indices, vertexCount,
		animateVertexCount, indicesCount, vertexStride, animateVertexStride, bit32Index))
	{
		buffer->drop();
		return nullptr;
	}

	return buffer;
}

IMeshBuffer* CD3D11ResourceFactory::createMeshBuffer(
	E_MEMORY_USAGE usage,
	void* vertices,
	void* indices,
	u32 vertexCount,
	u32 indicesCount,
	u32 vertexStride,
	bool bit32Index)
{
	CD3D11MeshBuffer* buffer = new CD3D11MeshBuffer(usage, md3dDevice, md3dDeviceContext, md3dDriver);
	if (!buffer)
	{
		GF_PRINT_CONSOLE_INFO("The CD3D11MeshBuffer created failed! Due to lack of memory!\n");
		return nullptr;
	}

	if (!buffer->init(vertices, indices, vertexCount, indicesCount, vertexStride, bit32Index))
	{
		buffer->drop();
		return nullptr;
	}

	return buffer;
}
