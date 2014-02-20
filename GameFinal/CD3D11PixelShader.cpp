#include "stdafx.h"
#include "CD3D11PixelShader.h"

bool CD3D11PixelShader::loadFromFile(const char* szFileName, const char* szFunctionName)
{
	HRESULT hr;
	if (!compile(szFileName, szFunctionName, EST_PIXEL_SHADER))
		return false;

	hr = md3dDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), NULL, &m_pd3dShader);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void CD3D11PixelShader::submit()
{
	u32 bufferCount = mConstantBuffers.size();

	md3dDeviceContext->PSSetConstantBuffers(0, bufferCount, md3dConstantBuffers);
	
	md3dDeviceContext->PSSetShader(m_pd3dShader, 0, 0);
}
