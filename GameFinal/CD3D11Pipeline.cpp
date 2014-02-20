#include "stdafx.h"
#include "CD3D11Pipeline.h"


CD3D11Pipeline::CD3D11Pipeline(const std::string& name,
	IShader** shaders,
	u32 shaderCount, 
	IInputLayout* inputlayout,
	E_PRIMITIVE_TYPE primitiveType, 
	CD3D11RenderState* renderState,
	ID3D11DeviceContext* pd3dDeviceContext, 
	CD3D11Driver* pd3d11Driver)
	:CPipeline(name, shaders, shaderCount, inputlayout, primitiveType)
	, mRenderState(renderState)
	, md3dDeviceContext(pd3dDeviceContext)
	, md3d11Driver(pd3d11Driver)
{
	mRenderState->grab();
}

CD3D11Pipeline::~CD3D11Pipeline()
{
	ReleaseReferenceCounted(mRenderState);
}


void CD3D11Pipeline::apply()
{
	//update the content of constant buffer
	for (u32 i = 0; i < EST_SHADER_COUNT; i++)
	{
		IShader* shader = mShaders[i];
		if (shader != nullptr)
		{
			shader->update();
			md3d11Driver->bindTexture(shader->getType(), shader->getTextureCount());
		}
	}


	CD3D11Driver::SD3D11DriverState& currentState = md3d11Driver->D3D11DriverState;

	/* if previous pipeline is the same, 
	then shaders, inputlayout, primivite type must also be same.
	so there is no need to modify if so. 
	NOTICE: the render state must be dealed with even if pipeline is same. */
	if (currentState.Pipeline != this)
	{
		currentState.Pipeline = this;

		// set shaders
		for (u32 i = 0; i < 5; i++)
		{
			IShader* shader = mShaders[i];
			if (shader != nullptr && currentState.Shaders[i] != shader)
			{
				shader->submit();
				currentState.Shaders[i] = shader;
			}
		}

		//set input layout (vertex format)
		if (currentState.InputLayout != mInputLayout)
		{
			mInputLayout->apply();
			currentState.InputLayout = mInputLayout;
		}

		//set primitive type
		if (currentState.PrimitiveType != mPrimitiveType)
		{
			md3d11Driver->bindPrimitiveType(mPrimitiveType);
			currentState.PrimitiveType = mPrimitiveType;
		}

		//set rasterizer state
		ID3D11RasterizerState* pd3dRasterizerState = mRenderState->getRasterizerState();
		if (currentState.RasterizerState != pd3dRasterizerState)
		{
			md3dDeviceContext->RSSetState(pd3dRasterizerState);
			currentState.RasterizerState = pd3dRasterizerState;
		}

		//set blend state
		ID3D11BlendState* pd3dBlendState = mRenderState->getBlendState();
		if (currentState.BlendState != pd3dBlendState)
		{
			const f32 blendFactor[] = { 0, 0, 0, 0 };
			md3dDeviceContext->OMSetBlendState(pd3dBlendState, blendFactor, 0xffffffff);
			currentState.BlendState = pd3dBlendState;
		}

		//set depth-stencil state
		ID3D11DepthStencilState* pd3dDepthStencilState = mRenderState->getDepthStencilState();
		if (currentState.DepthStencilState != pd3dDepthStencilState)
		{
			md3dDeviceContext->OMSetDepthStencilState(pd3dDepthStencilState, 0xff);
			currentState.DepthStencilState = pd3dDepthStencilState;
		}
	}

	

	
}

