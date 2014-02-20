#ifndef __D3D11_RENDER_STATE_CACHE_CLASS_H__
#define __D3D11_RENDER_STATE_CACHE_CLASS_H__

#include "IReferenceCounted.h"
#include "CSortCodeAllocator.h"

const u32 MAX_STATE_SORT_CODE = 255;

struct SD3D11RasterizerStateWrapper
{
	u32							SortCode;
	u32							HashCode;
	ID3D11RasterizerState*		RasterizerState;
};

struct SD3D11BlendStateWrapper
{
	u32							SortCode;
	u32							HashCode;
	ID3D11BlendState*			BlendState;
};

struct SD3D11DepthStencilStateWrapper
{
	u32							SortCode;
	u32							HashCode;
	ID3D11DepthStencilState*	DepthStencilState;
};

class CD3D11RenderStateCache : public IReferenceCounted
{
public:
	CD3D11RenderStateCache(ID3D11Device* pd3ddevice)
		:md3dDevice(pd3ddevice)
	{
		// init default rasterizer state
		mDefaultRasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
		mDefaultRasterizerStateDesc.CullMode = D3D11_CULL_BACK;
		mDefaultRasterizerStateDesc.FrontCounterClockwise = FALSE;
		mDefaultRasterizerStateDesc.DepthBias = 0;
		mDefaultRasterizerStateDesc.SlopeScaledDepthBias = 0.0f;
		mDefaultRasterizerStateDesc.DepthBiasClamp = 0.0f;
		mDefaultRasterizerStateDesc.DepthClipEnable = TRUE;
		mDefaultRasterizerStateDesc.ScissorEnable = FALSE;
		mDefaultRasterizerStateDesc.MultisampleEnable = FALSE;
		mDefaultRasterizerStateDesc.AntialiasedLineEnable = FALSE;
		getRasterizerState(mDefaultRasterizerStateDesc, mDefaultRasterizerStateWrapper);

		// init default blend state
		ZeroMemory(&mDefaultBlendStateDesc, sizeof(D3D11_BLEND_DESC));
		mDefaultBlendStateDesc.AlphaToCoverageEnable = FALSE;
		mDefaultBlendStateDesc.IndependentBlendEnable = FALSE;
		mDefaultBlendStateDesc.RenderTarget[0].BlendEnable = FALSE;
		mDefaultBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		mDefaultBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		mDefaultBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		mDefaultBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		mDefaultBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		mDefaultBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		mDefaultBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		getBlendState(mDefaultBlendStateDesc, mDefaultBlendStateWrapper);

		// init default depth-stencil state
		mDefaultDepthStencilStateDesc.DepthEnable = TRUE;
		mDefaultDepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		mDefaultDepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		mDefaultDepthStencilStateDesc.StencilEnable = FALSE;
		mDefaultDepthStencilStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		mDefaultDepthStencilStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		mDefaultDepthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		mDefaultDepthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		mDefaultDepthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		mDefaultDepthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		getDepthStencilState(mDefaultDepthStencilStateDesc, mDefaultDepthStencilStateWrapper);

	}

	u32 getRasterizerStateHashCode(const D3D11_RASTERIZER_DESC& desc)
	{
		/* | FrontCounterClockwise | FillMode | CullMode | DepthClipEnable | ScissorEnable | MultisampleEnable | AntialiasedLineEnable | 
		   |          1            |    1     |    11    |        1        |       1       |          1        |          1            |
		   
		   because the FillMode only have two values :  D3D11_FILL_WIREFRAME = 2, D3D11_FILL_SOLID = 3
		   so Fillmode only occupy 1 bit. thus the hash code only has 8 bits.
		   */

		return (desc.FrontCounterClockwise << 7)
			| ((desc.FillMode - 2) << 6)
			| (desc.CullMode << 4)
			| (desc.DepthClipEnable << 3)
			| (desc.ScissorEnable << 2)
			| (desc.MultisampleEnable << 1)
			| (desc.AntialiasedLineEnable);
	}

	u32 getBlendStateHashCode(const D3D11_BLEND_DESC& desc)
	{
		/* | AlphaToCoverageEnable | IndependentBlendEnable | BlendEnable[0] | SrcBlend[0] | DestBlend[0] | BlendOp[0] |
		   |		1			   |          1             |       1        |      5      |       5      |        3   |

		   | SrcBlendAlpha[0] | DestBlendAlpha[0] | BlendOpAlpha[0] |
		   |		5		  |          5        |       3         |
		   */

		return (desc.AlphaToCoverageEnable << 28)
			| (desc.IndependentBlendEnable << 27)
			| (desc.RenderTarget[0].BlendEnable << 26)
			| (desc.RenderTarget[0].SrcBlend << 21)
			| (desc.RenderTarget[0].DestBlend << 16)
			| (desc.RenderTarget[0].BlendOp << 13)
			| (desc.RenderTarget[0].SrcBlendAlpha << 8)
			| (desc.RenderTarget[0].DestBlendAlpha << 3)
			| (desc.RenderTarget[0].BlendOpAlpha);

	}

	u32 getDepthStencilStateHashCode(const D3D11_DEPTH_STENCIL_DESC& desc)
	{
		/*
		| DepthEnable | DepthWriteMask | DepthFunc | StencilEnable | StencilReadMask | StencilWriteMask |
		|     1       |		  1        |     3     |       1       |        8        |        8         |

		| FrontFace.StencilPassOp | FrontFace.StencilFailOp | FrontFace.StencilFailOp |
		|           3             |          3              |           3             |

		notice : I choose frontface fields, because front faces is more widely used than back faces.
		because StencilPassOp, StencilFailOp and StencilFailOp are all 1~8, so I 'AND(&)' 0x07 with them,
		to maintain they only occupy 3 bits.
		*/

		return (desc.DepthEnable << 30)
			| (desc.DepthWriteMask << 29)
			| (desc.DepthFunc << 26)
			| (desc.StencilEnable << 25)
			| (desc.StencilReadMask << 17)
			| (desc.StencilWriteMask << 9)
			| ((desc.FrontFace.StencilPassOp & 0x07) << 6)
			| ((desc.FrontFace.StencilFailOp & 0x07) << 3)
			| ((desc.FrontFace.StencilFailOp & 0x07));

	}


	bool equalsWithSameCode(const D3D11_RASTERIZER_DESC& desc, ID3D11RasterizerState* pd3dRasterState)
	{
		D3D11_RASTERIZER_DESC desc2;
		pd3dRasterState->GetDesc(&desc2);
		if (desc.DepthBias == desc2.DepthBias
			&& desc.DepthBiasClamp == desc2.DepthBiasClamp
			&& desc.SlopeScaledDepthBias == desc2.SlopeScaledDepthBias)
			return true;

		return false;
	}

	bool equalsWithSameCode(const D3D11_BLEND_DESC& desc, ID3D11BlendState* pd3dBlendState)
	{
		D3D11_BLEND_DESC desc2;
		pd3dBlendState->GetDesc(&desc2);

		if (desc.IndependentBlendEnable == FALSE)
			return true;

		return memcmp(&desc, &desc2, sizeof(desc)) == 0;
	}

	bool equalsWithSameCode(const D3D11_DEPTH_STENCIL_DESC& desc, ID3D11DepthStencilState* pd3dDepthStencilState)
	{
		D3D11_DEPTH_STENCIL_DESC desc2;
		pd3dDepthStencilState->GetDesc(&desc2);
		return memcmp(&desc, &desc2, sizeof(D3D11_DEPTH_STENCIL_DESC)) == 0;
	}

	bool getRasterizerState(const D3D11_RASTERIZER_DESC& desc, SD3D11RasterizerStateWrapper& wrapper, bool addIfNotFound = true)
	{
		// calcuate the hash code according to the desc.
		u32 code = getRasterizerStateHashCode(desc);

		auto it = mRasterizerStateWrappers.find(code);

		//if not have rasterizer state with same hash code in the cache.
		if (it == mRasterizerStateWrappers.end())
		{
			if (!addIfNotFound)
				return false;

			std::list<SD3D11RasterizerStateWrapper> stateList;
			wrapper.HashCode = code;
			wrapper.SortCode = mRasterizerStateCodeAllocator.allocate();
			md3dDevice->CreateRasterizerState(&desc, &wrapper.RasterizerState);
			stateList.push_back(wrapper);
			mRasterizerStateWrappers.insert(std::make_pair(code, stateList));
			return true;
		}

		// if have rasterizer state with the same code,
		// then search whether the state really equals.
		auto stateIt = it->second.begin();
		for (; stateIt != it->second.end(); stateIt++)
		{
			// if the two raster state really equals.
			// which means they have the same descs.
			if (equalsWithSameCode(desc, stateIt->RasterizerState))
			{
				wrapper = *stateIt;
				return true;
			}
		}

		// if not found, just create it, then add it to the list.
		wrapper.HashCode = code;
		wrapper.SortCode = mRasterizerStateCodeAllocator.allocate();
		md3dDevice->CreateRasterizerState(&desc, &wrapper.RasterizerState);
		it->second.push_back(wrapper);

		return true;
	}

	bool getBlendState(const D3D11_BLEND_DESC& desc, SD3D11BlendStateWrapper& wrapper, bool addIfNotFound = true)
	{
		u32 code = getBlendStateHashCode(desc);

		auto it = mBlendStateWrappers.find(code);

		//if not have blend state with same hash code in the cache.
		if (it == mBlendStateWrappers.end())
		{
			if (!addIfNotFound)
				return false;

			std::list<SD3D11BlendStateWrapper> stateList;
			wrapper.HashCode = code;
			wrapper.SortCode = mBlendStateCodeAllocator.allocate();
			md3dDevice->CreateBlendState(&desc, &wrapper.BlendState);
			stateList.push_back(wrapper);
			mBlendStateWrappers.insert(std::make_pair(code, stateList));
			return true;
		}

		// if have blend state with the same code,
		// then search whether the state really equals.
		auto stateIt = it->second.begin();
		for (; stateIt != it->second.end(); stateIt++)
		{
			// if the two blend state really equals.
			// (which means they have the same descs).
			if (equalsWithSameCode(desc, stateIt->BlendState))
			{
				wrapper = *stateIt;
				return true;
			}
		}

		// if not found, just create it, then add it to the list.
		wrapper.HashCode = code;
		wrapper.SortCode = mBlendStateCodeAllocator.allocate();
		md3dDevice->CreateBlendState(&desc, &wrapper.BlendState);
		it->second.push_back(wrapper);

		return true;
	}

	bool getDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc, SD3D11DepthStencilStateWrapper& wrapper, bool addIfNotFound = true)
	{
		u32 code = getDepthStencilStateHashCode(desc);

		auto it = mDepthStencilStateWrappers.find(code);

		//if not have depth-stencil state with same hash code in the cache.
		if (it == mDepthStencilStateWrappers.end())
		{
			if (!addIfNotFound)
				return false;

			std::list<SD3D11DepthStencilStateWrapper> stateList;
			wrapper.HashCode = code;
			wrapper.SortCode = mBlendStateCodeAllocator.allocate();
			md3dDevice->CreateDepthStencilState(&desc, &wrapper.DepthStencilState);
			stateList.push_back(wrapper);
			mDepthStencilStateWrappers.insert(std::make_pair(code, stateList));
			return true;
		}

		// if have blend state with the same code,
		// then search whether the state really equals.
		auto stateIt = it->second.begin();
		for (; stateIt != it->second.end(); stateIt++)
		{
			// if the two blend state really equals.
			// (which means they have the same descs).
			if (equalsWithSameCode(desc, stateIt->DepthStencilState))
			{
				wrapper = *stateIt;
				return true;
			}
		}

		// if not found, just create it, then add it to the list.
		wrapper.HashCode = code;
		wrapper.SortCode = mBlendStateCodeAllocator.allocate();
		md3dDevice->CreateDepthStencilState(&desc, &wrapper.DepthStencilState);
		it->second.push_back(wrapper);

		return true;
	}

	/*
	bool addRasterizerState(const D3D11_RASTERIZER_DESC& desc)
	{
		u32 code = getRasterizerStateHashCode(desc);
		auto it = .find(code);
		bool found = false;
		if (it == mRasterizerStates.end())
		{
			std::list<ID3D11RasterizerState*> vec;
			ID3D11RasterizerState* pd3dRasterizerState = NULL;
			md3dDevice->CreateRasterizerState(&desc, &pd3dRasterizerState);
			vec.push_back(pd3dRasterizerState);
			mRasterizerStates.insert(std::make_pair(code, vec));
		}
		else
		{
			auto vec_it = it->second.begin();
			for (; vec_it != it->second.end(); vec_it++)
			{
				ID3D11RasterizerState* pd3dRasterState2 = *vec_it;
				if (equalsWithSameCode(desc, pd3dRasterState2))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				ID3D11RasterizerState* pd3dRasterizerState = NULL;
				md3dDevice->CreateRasterizerState(&desc, &pd3dRasterizerState);
				it->second.push_back(pd3dRasterizerState);
			}
		}

		return !found;
	}
	*/

	SD3D11RasterizerStateWrapper getDefaultRasterizerState(D3D11_RASTERIZER_DESC& defaultDesc)
	{
		defaultDesc = mDefaultRasterizerStateDesc;
		return mDefaultRasterizerStateWrapper;
	}

	SD3D11BlendStateWrapper		getDefaultBlendState(D3D11_BLEND_DESC& defaultDesc)
	{
		defaultDesc = mDefaultBlendStateDesc;
		return mDefaultBlendStateWrapper;
	}

	SD3D11DepthStencilStateWrapper getDefaultDepthStencilState(D3D11_DEPTH_STENCIL_DESC& defaultDesc)
	{
		defaultDesc = mDefaultDepthStencilStateDesc;
		return mDefaultDepthStencilStateWrapper;
	}

private:
	ID3D11Device*						md3dDevice;

	/* for rasterizer state */
	D3D11_RASTERIZER_DESC				mDefaultRasterizerStateDesc;
	SD3D11RasterizerStateWrapper		mDefaultRasterizerStateWrapper;
	

	/* for blend state */
	D3D11_BLEND_DESC					mDefaultBlendStateDesc;
	SD3D11BlendStateWrapper				mDefaultBlendStateWrapper;

	/* for depth stencil state */
	D3D11_DEPTH_STENCIL_DESC			mDefaultDepthStencilStateDesc;
	SD3D11DepthStencilStateWrapper		mDefaultDepthStencilStateWrapper;


	/* (hashcode, state wrappers) mapping */
	std::map<u32, std::list<SD3D11RasterizerStateWrapper>>		mRasterizerStateWrappers;
	std::map<u32, std::list<SD3D11BlendStateWrapper>>			mBlendStateWrappers;
	std::map<u32, std::list<SD3D11DepthStencilStateWrapper>>	mDepthStencilStateWrappers;

	CSortCodeAllocator<MAX_STATE_SORT_CODE>					mRasterizerStateCodeAllocator;
	CSortCodeAllocator<MAX_STATE_SORT_CODE>					mBlendStateCodeAllocator;
	CSortCodeAllocator<MAX_STATE_SORT_CODE>					mDepthStencilStateCodeAllocator;
};

#endif