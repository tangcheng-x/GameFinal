#include "stdafx.h"
#include "CD3D11VertexShader.h"
#include "D3DUtil.h"

CD3D11VertexShader::~CD3D11VertexShader()
{
	ReleaseCOM(m_pd3dShader);
}

bool CD3D11VertexShader::loadFromFile(const char* szFileName, const char* szFunctionName)
{
	HRESULT hr;
	if (!compile(szFileName, szFunctionName, EST_VERTEX_SHADER))
		return false;

	hr = md3dDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), NULL, &m_pd3dShader);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void CD3D11VertexShader::submit()
{
	u32 bufferCount = mConstantBuffers.size();

	md3dDeviceContext->VSSetConstantBuffers(0, bufferCount, md3dConstantBuffers);

	md3dDeviceContext->VSSetShader(m_pd3dShader, 0, 0);
}

