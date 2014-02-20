#pragma once
#include "CPipeline.h"
#include "CD3D11Driver.h"
#include "CD3D11RenderState.h"

class CD3D11Pipeline : public CPipeline
{
public:
	CD3D11Pipeline(const std::string& name,
		IShader** shaders,
		u32 shaderCount,
		IInputLayout* inputlayout,
		E_PRIMITIVE_TYPE primitiveType,
		CD3D11RenderState* renderState,
		ID3D11DeviceContext* pd3dDeviceContext,
		CD3D11Driver* pd3d11Driver);

	IRenderState* getRenderState()
	{
		return mRenderState;
	}

	virtual void apply();


	virtual u64 getSortCode() const
	{
		/* vertex-shader 8bit
		pixel-shader 8bit
		input-layout 8bit
		renderstate  24bit
		*/

		return ((u64)mShaders[EST_VERTEX_SHADER]->getSortCode() << 40)
			| ((u64)mShaders[EST_PIXEL_SHADER]->getSortCode() << 32)
			| (mInputLayout->getSortCode() << 24)
			| (mRenderState->getSortCode());
	}

	virtual ~CD3D11Pipeline();

private:
	ID3D11DeviceContext*		md3dDeviceContext;
	CD3D11Driver*				md3d11Driver;
	CD3D11RenderState*			mRenderState;

};

