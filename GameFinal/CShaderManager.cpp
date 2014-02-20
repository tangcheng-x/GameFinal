#include "stdafx.h"
#include "CShaderManager.h"
#include "CD3D11Shader.h"
#include "CD3D11VertexShader.h"
#include "CD3D11PixelShader.h"

CShaderManager::CShaderManager(IResourceFactory* pResourceFactory)
:mResourceFactory(pResourceFactory)
{

}


IShader* CShaderManager::load(E_SHADER_TYPE shaderType,
	const std::string& filename,
	const std::string& functionName,
	const std::string& shaderName)
{
	auto it = mShadersMap.find(shaderName);
	if (it != mShadersMap.end())
	{
		return it->second;
	}

	u32 sortCode = mCodeAllocator[shaderType].allocate();
	IShader* shader = mResourceFactory->createShaderFromFile(shaderType, filename.c_str(), functionName.c_str(), sortCode, shaderName);
	if (!shader)
		return nullptr;

	mShadersMap.insert(std::make_pair(shaderName, shader));
	return shader;
}

IShader* CShaderManager::get(const std::string& name)
{
	auto it = mShadersMap.find(name);
	if (it != mShadersMap.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}
