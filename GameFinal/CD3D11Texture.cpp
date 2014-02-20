#include "stdafx.h"
#include "gfUtil.h"
#include "D3DUtil.h"
#include "CD3D11Texture.h"

CD3D11Texture::CD3D11Texture(ID3D11Device* pd3dDevice, 
	ID3D11DeviceContext* pd3dDeviceContext, 
	const std::string& name,
	u32 sortcode)
	:ITexture(name, sortcode)
	, md3dDevice(pd3dDevice)
	, md3dDeviceContext(pd3dDeviceContext)
	, m_pd3dTexture(NULL)
	, m_pd3dSRV(NULL)
{

}

bool CD3D11Texture::loadFromFile(const std::string& filename)
{
	HRESULT hr;
	ID3D11Texture2D* pd3dTexture = NULL;
	ID3D11ShaderResourceView* pd3dSRV = NULL;

	/*
	hr = D3DX11CreateShaderResourceViewFromFileA(md3dDevice, filename.c_str(),
		NULL, NULL, &pd3dSRV, NULL);
	if (FAILED(hr))
	{
		GF_PRINT_CONSOLE_INFO("create texture shader view (%s) failed.\n", filename.c_str());
		return false;
	}
	*/



	hr = D3DX11CreateTextureFromFileA(md3dDevice, filename.c_str(),
		NULL, NULL, (ID3D11Resource**)&pd3dTexture, NULL);
	if (FAILED(hr))
	{
		GF_PRINT_CONSOLE_INFO("load texture file (%s) failed.\n", filename.c_str());
		return false;
	}

	D3D11_TEXTURE2D_DESC texDesc;
	pd3dTexture->GetDesc(&texDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = texDesc.Format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = texDesc.MipLevels;
	desc.Texture2D.MostDetailedMip = 0;
	
	hr = md3dDevice->CreateShaderResourceView(pd3dTexture, &desc, &pd3dSRV);		
	if (FAILED(hr))
	{
		GF_PRINT_CONSOLE_INFO("create texture shader view (%s) failed.\n", filename.c_str());
		return false;
	}

	ReleaseCOM(m_pd3dTexture);
	ReleaseCOM(m_pd3dSRV);

	m_pd3dTexture = pd3dTexture;
	m_pd3dSRV = pd3dSRV;

	return true;
}


CD3D11Texture::~CD3D11Texture()
{
	ReleaseCOM(m_pd3dTexture);
	ReleaseCOM(m_pd3dSRV);
}
