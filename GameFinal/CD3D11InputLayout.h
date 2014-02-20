#ifndef __CD3D11INPUTLAYOUT_H__
#define __CD3D11INPUTLAYOUT_H__

#include "IInputLayout.h"
#include "D3DUtil.h"

class CD3D11InputLayout : public IInputLayout
{
public:
	CD3D11InputLayout(u32 sortCode, 
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pd3dDeviceContext,
		ID3D10Blob* vertexShaderBuffer,
		const std::vector<SInputLayoutElement>& elements, u32 hashcode);

	virtual void apply();

	virtual ~CD3D11InputLayout();

private:
	ID3D11DeviceContext*	md3dDeviceContext;
	ID3D11InputLayout*		m_pd3dInputLayout;
};

#endif
