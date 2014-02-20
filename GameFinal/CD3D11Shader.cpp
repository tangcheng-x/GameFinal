#include "stdafx.h"
#include "CD3D11Shader.h"
#include "D3DUtil.h"


bool CD3D11Shader::compile(const char* szFileName, const char* szFunctionName, E_SHADER_TYPE type)
{
	HRESULT hr;

	const char* szProfile = getShaderProfile();
	ID3D10Blob* errorMessage = NULL;

	hr = D3DX11CompileFromFileA(szFileName, NULL, NULL, szFunctionName, szProfile, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&mShaderBuffer, &errorMessage, NULL);
	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			GF_PRINT_CONSOLE_INFO((char*)errorMessage->GetBufferPointer());
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			GF_PRINT_CONSOLE_INFO("load shader file('%s') and function ('%s') failed!\n", szFileName, szFunctionName);
		}
		return false;
	}

	initContext();
	
	return true;
}

bool CD3D11Shader::initContext()
{
	HRESULT hr;
	ID3D11ShaderReflection* shaderReflection = NULL;

	memset(md3dConstantBuffers, 0, sizeof(md3dConstantBuffers));

	hr = D3DReflect(mShaderBuffer->GetBufferPointer(),
		mShaderBuffer->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&shaderReflection);

	if (FAILED(hr))
	{
		throw std::runtime_error("Create Shader Reflection Failed!");
	}

	D3D11_SHADER_DESC shaderDesc;
	shaderReflection->GetDesc(&shaderDesc);

	ID3D11ShaderReflectionVariable* var = shaderReflection->GetVariableByName("gTexture1");

	D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
	s32 uiMaxBindPoint = -1;
	for (u32 i = 0; i < shaderDesc.BoundResources; i++)
	{
		shaderReflection->GetResourceBindingDesc(i, &resourceDesc);

		std::cout << resourceDesc.Name << std::endl;

		if (resourceDesc.Type == D3D10_SIT_TEXTURE)
		{
			std::string name = resourceDesc.Name;
			mShaderResourceDescs.insert(std::make_pair(name, resourceDesc));

			s32 bindPoint = resourceDesc.BindPoint;
			if (bindPoint > uiMaxBindPoint)
				uiMaxBindPoint = bindPoint;
		}
	}

	if (uiMaxBindPoint >= 0)
		mSrvNum = uiMaxBindPoint + 1;
	else
		mSrvNum = 0;

	for (u32 i = 0; i < shaderDesc.ConstantBuffers; i++)
	{
		ID3D11ShaderReflectionConstantBuffer* cbReflection = NULL;
		cbReflection = shaderReflection->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC cbDesc;
		cbReflection->GetDesc(&cbDesc);

		SShaderConstantBuffer* pCbStruct = new SShaderConstantBuffer(cbDesc.Name);
		pCbStruct->Size = cbDesc.Size;

		D3D11_BUFFER_DESC bufDesc;
		bufDesc.ByteWidth = cbDesc.Size;
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufDesc.MiscFlags = 0;
		bufDesc.StructureByteStride = 0;

		md3dDevice->CreateBuffer(&bufDesc, 0, &md3dConstantBuffers[i]);

		pCbStruct->pData = malloc(cbDesc.Size);

		std::unique_ptr<SShaderConstantBuffer> cbPtr(pCbStruct);
		mConstantBuffers.push_back(std::move(cbPtr));

		u32 constantBufferIndex = mConstantBuffers.size() - 1;

		for (u32 j = 0; j < cbDesc.Variables; j++)
		{
			ID3D11ShaderReflectionVariable* reflectionVariable = cbReflection->GetVariableByIndex(j);
			if (reflectionVariable)
			{
				SShaderConstantVariable scv;
				reflectionVariable->GetDesc(&scv.Desc);

				ID3D11ShaderReflectionType* reflectionType = reflectionVariable->GetType();

				reflectionType->GetDesc(&scv.TypeDesc);

				std::string varname = scv.Desc.Name;

				if (mConstantVariables.find(varname) != mConstantVariables.end())
				{
					throw std::runtime_error("put the variable with same name into one shader file!");
				}

				scv.ConstantBufferIndex = constantBufferIndex;
				mConstantVariables.insert(std::make_pair(varname, scv));
			}
		}
	}

	ReleaseCOM(shaderReflection);

	return true;
}

SShaderConstantVariable* CD3D11Shader::getConstantVariable(const std::string& varname)
{
	auto it = mConstantVariables.find(varname);
	if (it == mConstantVariables.end())
		return nullptr;

	return &(it->second);
}

bool CD3D11Shader::setRawData(const std::string& varname, void* data, u32 size, bool ignoreIfAlreadyUpdate)
{
	SShaderConstantVariable* cv = getConstantVariable(varname);
	if (cv == nullptr)
		return false;

	if (ignoreIfAlreadyUpdate && cv->AlreadyUpdated)
		return false;

	u8* pData = reinterpret_cast<u8*>(mConstantBuffers[cv->ConstantBufferIndex]->pData);
	memcpy(pData + cv->Desc.StartOffset, data, size);
	mConstantBuffers[cv->ConstantBufferIndex]->Dirty = true;
	cv->AlreadyUpdated = true;
	return true;
}

bool CD3D11Shader::setFloat(const std::string& varname, f32 val, bool ignoreIfAlreadyUpdate)
{
	SShaderConstantVariable* cv = getConstantVariable(varname);
	if (cv == nullptr)
		return false;

	if (ignoreIfAlreadyUpdate && cv->AlreadyUpdated)
		return false;

	if (cv->TypeDesc.Class != D3D10_SVC_SCALAR ||
		cv->TypeDesc.Type != D3D10_SVT_FLOAT)
	{
		return false;
	}
	return setRawData(cv, &val, sizeof(f32));
}

bool CD3D11Shader::setVector(const std::string& varname, const f32* val, bool ignoreIfAlreadyUpdate)
{
	SShaderConstantVariable* cv = getConstantVariable(varname);
	if (cv == nullptr)
		return false;

	if (ignoreIfAlreadyUpdate && cv->AlreadyUpdated)
		return false;

	if (cv->TypeDesc.Class != D3D10_SVC_VECTOR ||
		cv->TypeDesc.Type != D3D10_SVT_FLOAT)
	{
		return false;
	}

	return setRawData(cv, (void*)val, sizeof(f32)* cv->TypeDesc.Columns);
}

bool CD3D11Shader::setMatrix(const std::string& varname, const f32* matrix, bool ignoreIfAlreadyUpdate)
{
	SShaderConstantVariable* cv = getConstantVariable(varname);
	if (cv == nullptr)
		return false;

	if (ignoreIfAlreadyUpdate && cv->AlreadyUpdated)
		return false;

	if (cv->TypeDesc.Class != D3D10_SVC_MATRIX_COLUMNS ||
		cv->TypeDesc.Type != D3D10_SVT_FLOAT ||
		cv->TypeDesc.Columns != 4 ||
		cv->TypeDesc.Rows != 4)
	{
		return false;
	}
	XMFLOAT4X4 m = *reinterpret_cast<const XMFLOAT4X4*>(matrix);
	XMMATRIX mat = XMLoadFloat4x4(&m);
	mat = XMMatrixTranspose(mat);

	return setRawData(cv, (void*)&mat, sizeof(f32)* 16);
}

bool CD3D11Shader::setMatrix(const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdate)
{
	SShaderConstantVariable* cv = getConstantVariable(varname);
	if (cv == nullptr)
		return false;

	if (ignoreIfAlreadyUpdate && cv->AlreadyUpdated)
		return false;

	if (cv->TypeDesc.Class != D3D10_SVC_MATRIX_COLUMNS ||
		cv->TypeDesc.Type != D3D10_SVT_FLOAT ||
		cv->TypeDesc.Columns != 4 ||
		cv->TypeDesc.Rows != 4)
	{
		return false;
	}
	XMMATRIX m = XMMatrixTranspose(matrix);
	return setRawData(cv, (void*)&m, sizeof(f32)* 16);
}

bool CD3D11Shader::setMatrix(const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdate)
{
	SShaderConstantVariable* cv = getConstantVariable(varname);
	if (cv == nullptr)
		return false;

	if (ignoreIfAlreadyUpdate && cv->AlreadyUpdated)
		return false;

	if (cv->TypeDesc.Class != D3D10_SVC_MATRIX_COLUMNS ||
		cv->TypeDesc.Type != D3D10_SVT_FLOAT ||
		cv->TypeDesc.Columns != 4 ||
		cv->TypeDesc.Rows != 4)
	{
		return false;
	}
	XMMATRIX m = XMLoadFloat4x4(&matrix);
	XMMATRIX transposeMat = XMMatrixTranspose(m);
	return setRawData(cv, (void*)&transposeMat, sizeof(f32)* 16);
}

bool CD3D11Shader::setTransposedMatrixArray(const std::string& varname, const f32* matrixs, u32 count, bool ignoreIfAlreadyUpdate)
{
	SShaderConstantVariable* cv = getConstantVariable(varname);
	if (cv == nullptr)
		return false;

	if (ignoreIfAlreadyUpdate && cv->AlreadyUpdated)
		return false;

	if (cv->TypeDesc.Class != D3D10_SVC_MATRIX_COLUMNS ||
		cv->TypeDesc.Type != D3D10_SVT_FLOAT ||
		cv->TypeDesc.Columns != 4 ||
		cv->TypeDesc.Rows != 4 || 
		cv->TypeDesc.Elements == 0)
	{
		return false;
	}

	return setRawData(cv, (void*)matrixs, sizeof(XMFLOAT4X4)* count);
}

bool CD3D11Shader::setTexture(const std::string& varname, ITexture* texture)
{
	auto it = mShaderResourceDescs.find(varname);
	if (it == mShaderResourceDescs.end())
		return false;

	const D3D11_SHADER_INPUT_BIND_DESC& desc = it->second;
	if (desc.Type != D3D10_SIT_TEXTURE)
		return false;

	u32 bindPoint = desc.BindPoint;
	md3dDriver->setTexture(getType(), bindPoint, texture);

	return true;
}

bool CD3D11Shader::setRawData(SShaderConstantVariable* cv, void* data, u32 size)
{
	u8* pData = reinterpret_cast<u8*>(mConstantBuffers[cv->ConstantBufferIndex]->pData);
	memcpy(pData + cv->Desc.StartOffset, data, size);
	mConstantBuffers[cv->ConstantBufferIndex]->Dirty = true;
	cv->AlreadyUpdated = true;
	return true;
}

bool CD3D11Shader::applyConstantBuffer(ID3D11DeviceContext* pd3dDeviceContext, u32 index)
{
	HRESULT hr;
	SShaderConstantBuffer* pConstantBuffer = mConstantBuffers[index].get();
	ID3D11Buffer* cbBuffer = md3dConstantBuffers[index];
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = pd3dDeviceContext->Map(cbBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
		return false;

	memcpy(mappedResource.pData, pConstantBuffer->pData, pConstantBuffer->Size);

	pd3dDeviceContext->Unmap(cbBuffer, 0);

	return true;
}

// 一定调用
void CD3D11Shader::update(bool forceUpdate /* = false */)
{
	u32 constantBufferCount = mConstantBuffers.size();
	for (u32 i = 0; i < constantBufferCount; i++)
	{
		SShaderConstantBuffer* pcb = mConstantBuffers[i].get();
		if (pcb->Dirty)
		{
			applyConstantBuffer(md3dDeviceContext, i);
			pcb->Dirty = false;
		}
	}
}

void CD3D11Shader::reset()
{
	auto it = mConstantVariables.begin();
	for (; it != mConstantVariables.end(); it++)
	{
		it->second.AlreadyUpdated = false;
	}
}

bool CD3D11Shader::isAlreadyUpdated(const std::string& varname)
{
	auto it = mConstantVariables.find(varname);
	if (it == mConstantVariables.end())
		return true;

	return it->second.AlreadyUpdated;
}
