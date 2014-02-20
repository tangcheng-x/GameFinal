#ifndef __D3D11_RENDER_STATE_CLASS_H__
#define __D3D11_RENDER_STATE_CLASS_H__

#include "IRenderState.h"
#include "CD3D11RenderStateCache.h"


class CD3D11RenderState : public IRenderState
{
public:
	CD3D11RenderState(const std::string& name, CD3D11RenderStateCache* d3dRenderStateCache);
	virtual ~CD3D11RenderState();

	virtual void set(E_RENDER_STATE state, u32 value);
	virtual void setFloat(E_RENDER_STATE state, f32 value);
	virtual void confirm();
	virtual void cancel();

	ID3D11RasterizerState*		getRasterizerState()
	{
		return mRasterizerStateWrapper.RasterizerState;
	}

	ID3D11BlendState*			getBlendState()
	{
		return mBlendStateWrapper.BlendState;
	}

	ID3D11DepthStencilState*	getDepthStencilState()
	{
		return mDepthStencilStateWrapper.DepthStencilState;
	}

	D3D11_BLEND convertBlendMode(E_BLEND_TYPE blend)
	{
		switch (blend)
		{
		case E_BLEND_ZERO:				return D3D11_BLEND_ZERO;
		case E_BLEND_ONE:				return D3D11_BLEND_ONE;
		case E_BLEND_SRC_COLOR:			return D3D11_BLEND_SRC_COLOR;
		case E_BLEND_INV_SRC_COLOR:		return D3D11_BLEND_INV_SRC_COLOR;
		case E_BLEND_SRC_ALPHA:			return D3D11_BLEND_SRC_ALPHA;
		case E_BLEND_INV_SRC_ALPHA:		return D3D11_BLEND_INV_SRC_ALPHA;
		case E_BLEND_DEST_ALPHA:		return D3D11_BLEND_DEST_ALPHA;
		case E_BLEND_INV_DEST_ALPHA:	return D3D11_BLEND_INV_DEST_ALPHA;
		case E_BLEND_DEST_COLOR:		return D3D11_BLEND_DEST_COLOR;
		case E_BLEND_INV_DEST_COLOR:	return D3D11_BLEND_INV_DEST_COLOR;
		case E_BLEND_SRC_ALPHA_SAT:		return D3D11_BLEND_SRC_ALPHA_SAT;
		case E_BLEND_BLEND_FACTOR:		return D3D11_BLEND_BLEND_FACTOR;
		case E_BLEND_INV_BLEND_FACTOR:	return D3D11_BLEND_INV_BLEND_FACTOR;
		case E_BLEND_SRC1_COLOR:		return D3D11_BLEND_SRC1_COLOR;
		case E_BLEND_INV_SRC1_COLOR:	return D3D11_BLEND_INV_SRC1_COLOR;
		case E_BLEND_SRC1_ALPHA:		return D3D11_BLEND_SRC1_ALPHA;
		case E_BLEND_INV_SRC1_ALPHA:	return D3D11_BLEND_INV_SRC1_ALPHA;
		}
		return D3D11_BLEND_ONE;
	}

	D3D11_BLEND_OP convertBlendOp(E_BLEND_OP op)
	{
		switch (op)
		{
		case EBO_ADD:			return D3D11_BLEND_OP_ADD;
		case EBO_SUBTRACT:		return D3D11_BLEND_OP_SUBTRACT;
		case EBO_REV_SUBTRACT:	return D3D11_BLEND_OP_REV_SUBTRACT;
		case EBO_MIN:			return D3D11_BLEND_OP_MIN;
		case EBO_MAX:			return D3D11_BLEND_OP_MAX;
		}
		return D3D11_BLEND_OP_ADD;
	}

	D3D11_COMPARISON_FUNC convertComparisonFunc(E_COMPARISON_FUNC func)
	{
		switch (func)
		{
		case ECF_NEVER: return D3D11_COMPARISON_NEVER;
		case ECF_LESS: return D3D11_COMPARISON_LESS;
		case ECF_EQUAL: return D3D11_COMPARISON_EQUAL;
		case ECF_LESS_EQUAL: return D3D11_COMPARISON_LESS_EQUAL;
		case ECF_GREATER: return D3D11_COMPARISON_GREATER;
		case ECF_NOT_EQUAL: return D3D11_COMPARISON_NOT_EQUAL;
		case ECF_GREATER_EQUAL: return D3D11_COMPARISON_GREATER_EQUAL;
		case ECF_ALWAYS: return D3D11_COMPARISON_ALWAYS;
		}
		return D3D11_COMPARISON_ALWAYS;
	}

	D3D11_STENCIL_OP convertStencilOp(E_STENCIL_OP op)
	{
		switch (op)
		{
		case ESO_KEEP: return D3D11_STENCIL_OP_KEEP;
		case ESO_ZERO: return D3D11_STENCIL_OP_ZERO;
		case ESO_REPLACE:return D3D11_STENCIL_OP_REPLACE;
		case ESO_INCR_SAT: return D3D11_STENCIL_OP_INCR_SAT;
		case ESO_DECR_SAT: return D3D11_STENCIL_OP_DECR_SAT;
		case ESO_INVERT: return D3D11_STENCIL_OP_INVERT;
		case ESO_INCR: return D3D11_STENCIL_OP_INCR;
		case ESO_DECR: return D3D11_STENCIL_OP_DECR;
		}
		return D3D11_STENCIL_OP_KEEP;
	}

	void calcSortCode()
	{
		mSortCode = (mRasterizerStateWrapper.SortCode << 16) | (mBlendStateWrapper.SortCode) | (mDepthStencilStateWrapper.SortCode);
	}

private:
	CD3D11RenderStateCache*			md3dRenderStateCache;

	bool							mRasterStateDirty;
	bool							mBlendStateDirty;
	bool							mDepthStencilStateDirty;

	SD3D11RasterizerStateWrapper	mRasterizerStateWrapper;
	SD3D11BlendStateWrapper			mBlendStateWrapper;
	SD3D11DepthStencilStateWrapper	mDepthStencilStateWrapper;

	D3D11_RASTERIZER_DESC			mRasterizerDesc;
	D3D11_BLEND_DESC				mBlendDesc;
	D3D11_DEPTH_STENCIL_DESC		mDepthStencilDesc;


};

#endif
