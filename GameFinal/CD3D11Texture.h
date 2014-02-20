#ifndef __CD3D11TEXTURE_H_INCLUDE__
#define __CD3D11TEXTURE_H_INCLUDE__

#include "ITexture.h"

class CD3D11Texture : public ITexture
{
public:
	CD3D11Texture(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, const std::string& name, u32 sortCode);

	bool loadFromFile(const std::string& filename);

	ID3D11ShaderResourceView* getShaderResourceView()
	{
		return m_pd3dSRV;
	}

	virtual ~CD3D11Texture();
private:
	ID3D11Device*					md3dDevice;
	ID3D11DeviceContext*			md3dDeviceContext;
	ID3D11Texture2D*				m_pd3dTexture;
	ID3D11ShaderResourceView*		m_pd3dSRV;
};

#endif
