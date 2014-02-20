#ifndef __CD3D11VERTEXSHADER_H__
#define __CD3D11VERTEXSHADER_H__

#include "CD3D11Shader.h"

class CD3D11VertexShader : public CD3D11Shader
{
public:
	CD3D11VertexShader(u32 id,
		std::string name,
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pd3dDeviceContext,
		CD3D11Driver* pd3dDriver)
		:CD3D11Shader(id, name, pd3dDevice, pd3dDeviceContext, pd3dDriver)
		, m_pd3dShader(nullptr)
	{

	}

	virtual E_SHADER_TYPE getType() const
	{
		return EST_VERTEX_SHADER;
	}

	virtual const char* getShaderProfile() const
	{
		return "vs_5_0";
	}

	virtual bool loadFromFile(const char* szFileName, const char* szFunctionName);

	virtual void submit();

	virtual ~CD3D11VertexShader();

private:

	ID3D11VertexShader*			m_pd3dShader;
};

#endif

