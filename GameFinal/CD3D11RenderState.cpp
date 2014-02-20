#include "stdafx.h"
#include "CD3D11RenderState.h"


CD3D11RenderState::CD3D11RenderState(const std::string& name, 
	CD3D11RenderStateCache* d3dRenderStateCache)
: IRenderState(name)
, mRasterStateDirty(false)
, mBlendStateDirty(false)
, mDepthStencilStateDirty(false)
, md3dRenderStateCache(d3dRenderStateCache)
{
	mRasterizerStateWrapper = md3dRenderStateCache->getDefaultRasterizerState(mRasterizerDesc);
	mBlendStateWrapper = md3dRenderStateCache->getDefaultBlendState(mBlendDesc);
	mDepthStencilStateWrapper = md3dRenderStateCache->getDefaultDepthStencilState(mDepthStencilDesc);
}


CD3D11RenderState::~CD3D11RenderState()
{

}


void CD3D11RenderState::set(E_RENDER_STATE state, u32 value)
{
	switch (state)
	{
	/* for rasterizer state */
	case ERS_FILL_MODE:
		{
			D3D11_FILL_MODE fillMode;
			if (value == E_FILL_SOLID)
				fillMode = D3D11_FILL_SOLID;
			else
				fillMode = D3D11_FILL_WIREFRAME;

			if (mRasterizerDesc.FillMode != fillMode)
			{
				mRasterizerDesc.FillMode = fillMode;
				mRasterStateDirty = true;
			}
		}
		break;
	case ERS_CULL_MODE:
		{
			 D3D11_CULL_MODE cullMode;
			 BOOL frontCounterClockwise;
			 if (value == E_CULL_NONE)
			 {
				 cullMode = D3D11_CULL_NONE;
				 frontCounterClockwise = FALSE;
			 }
			 else if (value == E_CULL_CW)
			 {
				 cullMode = D3D11_CULL_BACK;
				 frontCounterClockwise = TRUE;
			 }
			 else
			 {
				 cullMode = D3D11_CULL_BACK;
				 frontCounterClockwise = FALSE;
			 }
			 if (mRasterizerDesc.CullMode != cullMode || mRasterizerDesc.FrontCounterClockwise != frontCounterClockwise)
			 {
				 mRasterizerDesc.CullMode = cullMode;
				 mRasterizerDesc.FrontCounterClockwise = frontCounterClockwise;
				 mRasterStateDirty = true;
			 }
		}
		break;
	case ERS_DEPTH_BIAS:
		{
			s32 depthBias = static_cast<s32>(value);
			if (mRasterizerDesc.DepthBias != depthBias)
			{
				mRasterizerDesc.DepthBias = depthBias;
				mRasterStateDirty = true;
			}
		}
		break;
	case ERS_DEPTH_BIAS_CLAMP:
		{
			f32 depthBiasClamp = static_cast<f32>(value);
			if (mRasterizerDesc.DepthBiasClamp != depthBiasClamp)
			{
				mRasterizerDesc.DepthBiasClamp = depthBiasClamp;
				mRasterStateDirty = true;
			}
		}
		break;
	case ERS_SCISSOR_ENABLE:
		if (mRasterizerDesc.ScissorEnable != value)
		{
			mRasterizerDesc.ScissorEnable = value;
			mRasterStateDirty = true;
		}
		break;
	case ERS_MULTISAMPLE_ENABLE:
		if (mRasterizerDesc.MultisampleEnable != value)
		{
			mRasterizerDesc.MultisampleEnable = value;
			mRasterStateDirty = true;
		}
		break;
	case ERS_ANTI_LINE_ENABLE:
		if (mRasterizerDesc.AntialiasedLineEnable != value)
		{
			mRasterizerDesc.AntialiasedLineEnable = value;
			mRasterStateDirty = true;
		}
		break;

		/* for blend state */
	case ERS_BLEND_ENABLE:
		if (mBlendDesc.RenderTarget[0].BlendEnable != value)
		{
			mBlendDesc.RenderTarget[0].BlendEnable = value;
			mBlendStateDirty = true;
		}
		break;
	case ERS_SRC_BLEND:
		{
			D3D11_BLEND blend = convertBlendMode((E_BLEND_TYPE)value);
			if (mBlendDesc.RenderTarget[0].SrcBlend != blend)
			{
				mBlendDesc.RenderTarget[0].SrcBlend = blend;
				mBlendStateDirty = true;
			} 
		}
		break;
	case ERS_DEST_BLEND:
		{
			D3D11_BLEND blend = convertBlendMode((E_BLEND_TYPE)value);
			if (mBlendDesc.RenderTarget[0].DestBlend != blend)
			{
				mBlendDesc.RenderTarget[0].DestBlend = blend;
				mBlendStateDirty = true;
			}
		}
		break;
	case ERS_BLEND_OP:
		{
			D3D11_BLEND_OP op = convertBlendOp((E_BLEND_OP)value);
			if (mBlendDesc.RenderTarget[0].BlendOp != op)
			{
				mBlendDesc.RenderTarget[0].BlendOp = op;
				mBlendStateDirty = true;
			}
		}
		break;
	case ERS_SRC_BLEND_ALPHA:
		{
			D3D11_BLEND blend = convertBlendMode((E_BLEND_TYPE)value);
			if (mBlendDesc.RenderTarget[0].SrcBlendAlpha != blend)
			{
				mBlendDesc.RenderTarget[0].SrcBlendAlpha = blend;
				mBlendStateDirty = true;
			}
		}
		break;
	case ERS_DEST_BLEND_ALPHA:
		{
			D3D11_BLEND blend = convertBlendMode((E_BLEND_TYPE)value);
			if (mBlendDesc.RenderTarget[0].DestBlendAlpha != blend)
			{
				mBlendDesc.RenderTarget[0].DestBlendAlpha = blend;
				mBlendStateDirty = true;
			}
		}
		break;
	case ERS_BLEND_OP_ALPHA:
		{
			D3D11_BLEND_OP op = convertBlendOp((E_BLEND_OP)value);
			if (mBlendDesc.RenderTarget[0].BlendOpAlpha != op)
			{
				mBlendDesc.RenderTarget[0].BlendOpAlpha = op;
				mBlendStateDirty = true;
			}
		}
		break;
	case ERS_BLEND_WRITE_MASK:
		{
			UINT8 writeMask = static_cast<UINT8>(value & 0xFF);
			if (mBlendDesc.RenderTarget[0].RenderTargetWriteMask != writeMask)
			{
				mBlendDesc.RenderTarget[0].RenderTargetWriteMask = writeMask;
				mBlendStateDirty = true;
			}
		}
		break;
	case ERS_ALPHA_COVERAGE_ENABLE:
		if (mBlendDesc.AlphaToCoverageEnable != value)
		{
			mBlendDesc.AlphaToCoverageEnable = value;
			mBlendStateDirty = true;
		}
		break;
	case ERS_INDEPENDENT_BLEND_ENABLE:
		if (mBlendDesc.IndependentBlendEnable != value)
		{
			mBlendDesc.IndependentBlendEnable = value;
			mBlendStateDirty = true;
		}
		break;

		/* for depth-stencil state */
	case ERS_DEPTH_ENABLE:
		if (mDepthStencilDesc.DepthEnable != value)
		{
			mDepthStencilDesc.DepthEnable = value;
			mDepthStencilStateDirty = true;
		}
		break;
	case ERS_DEPTH_WRITE_ENABLE:
		{
			D3D11_DEPTH_WRITE_MASK depthWriteMask = (value) ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			if (mDepthStencilDesc.DepthWriteMask != depthWriteMask)
			{
				mDepthStencilDesc.DepthWriteMask = depthWriteMask;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_DEPTH_FUNC:
		{
			D3D11_COMPARISON_FUNC func = convertComparisonFunc((E_COMPARISON_FUNC)value);
			if (mDepthStencilDesc.DepthFunc != func)
			{
				mDepthStencilDesc.DepthFunc = func;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_STENCIL_ENABLE:
		if (mDepthStencilDesc.StencilEnable != value)
		{
			mDepthStencilDesc.StencilEnable = value;
			mDepthStencilStateDirty = true;
		}
		break;
	case ERS_STENCIL_READ_MASK:
		{
			u8 stencilReadMask = static_cast<u8>(value & 0xFF);
			if (mDepthStencilDesc.StencilReadMask != stencilReadMask)
			{
				mDepthStencilDesc.StencilReadMask = stencilReadMask;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_STENCIL_WRITE_MASK:
		{
			u8 stencilWriteMask = static_cast<u8>(value & 0xFF);
			if (mDepthStencilDesc.StencilWriteMask != stencilWriteMask)
			{
				mDepthStencilDesc.StencilWriteMask = stencilWriteMask;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_STENCIL_FAIL_OP:
		{
			D3D11_STENCIL_OP op = convertStencilOp((E_STENCIL_OP)value);
			if (mDepthStencilDesc.FrontFace.StencilFailOp != op)
			{
				mDepthStencilDesc.FrontFace.StencilFailOp = op;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_STENCIL_DEPTH_FAIL_OP:
		{
			D3D11_STENCIL_OP op = convertStencilOp((E_STENCIL_OP)value);
			if (mDepthStencilDesc.FrontFace.StencilDepthFailOp != op)
			{
				mDepthStencilDesc.FrontFace.StencilDepthFailOp = op;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_STENCIL_PASS_OP:
		{
			D3D11_STENCIL_OP op = convertStencilOp((E_STENCIL_OP)value);
			if (mDepthStencilDesc.FrontFace.StencilPassOp != op)
			{
				mDepthStencilDesc.FrontFace.StencilPassOp = op;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_BACKFACE_STENCIL_FAIL_OP:
		{
			D3D11_STENCIL_OP op = convertStencilOp((E_STENCIL_OP)value);
			if (mDepthStencilDesc.BackFace.StencilFailOp != op)
			{
				mDepthStencilDesc.BackFace.StencilFailOp = op;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_BACKFACE_STENCIL_DEPTH_FAIL_OP:
		{
			D3D11_STENCIL_OP op = convertStencilOp((E_STENCIL_OP)value);
			if (mDepthStencilDesc.BackFace.StencilDepthFailOp != op)
			{
				mDepthStencilDesc.BackFace.StencilDepthFailOp = op;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	case ERS_BACKFACE_STENCIL_PASS_OP:
		{
			D3D11_STENCIL_OP op = convertStencilOp((E_STENCIL_OP)value);
			if (mDepthStencilDesc.BackFace.StencilPassOp != op)
			{
				mDepthStencilDesc.BackFace.StencilPassOp = op;
				mDepthStencilStateDirty = true;
			}
		}
		break;
	}

}

void CD3D11RenderState::setFloat(E_RENDER_STATE state, f32 value)
{
	switch (state)
	{
	case ERS_DEPTH_BIAS_CLAMP:
		{
			if (mRasterizerDesc.DepthBiasClamp != value)
			{
				mRasterizerDesc.DepthBiasClamp = value;
				mRasterStateDirty = true;
			}
		}
		break;
	}
}

void CD3D11RenderState::confirm()
{
	if (mRasterStateDirty)
	{
		md3dRenderStateCache->getRasterizerState(mRasterizerDesc, mRasterizerStateWrapper);
		mRasterStateDirty = false;
	}

	if (mBlendStateDirty)
	{
		md3dRenderStateCache->getBlendState(mBlendDesc, mBlendStateWrapper);
		mBlendStateDirty = false;
	}

	if (mDepthStencilStateDirty)
	{
		md3dRenderStateCache->getDepthStencilState(mDepthStencilDesc, mDepthStencilStateWrapper);
		mDepthStencilStateDirty = false;
	}

	calcSortCode();
}

void CD3D11RenderState::cancel()
{
	if (mRasterStateDirty)
	{
		mRasterizerStateWrapper.RasterizerState->GetDesc(&mRasterizerDesc);
		mRasterStateDirty = false;
	}

	if (mBlendStateDirty)
	{
		mBlendStateWrapper.BlendState->GetDesc(&mBlendDesc);
		mBlendStateDirty = false;
	}

	if (mDepthStencilStateDirty)
	{
		mDepthStencilStateWrapper.DepthStencilState->GetDesc(&mDepthStencilDesc);
		mDepthStencilStateDirty = false;
	}
}
