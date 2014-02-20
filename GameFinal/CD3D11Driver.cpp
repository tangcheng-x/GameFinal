#include "stdafx.h"
#include "CD3D11Driver.h"
#include "CD3D11SimpleMesh.h"
#include "CD3D11ResourceFactory.h"
#include "CD3D11Texture.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CMaterialManager.h"
#include "CRenderStateManager.h"
#include "CShaderManager.h"
#include "CInputLayoutManager.h"
#include "CPipelineManager.h"
#include "CMeshManager.h"
#include "CGeometryCreator.h"
#include "CResourceGroupManager.h"

CD3D11Driver::CD3D11Driver(IDevice* device)
:mDevice(device)
{

}



bool CD3D11Driver::init()
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;

	const SCreationParameters& params = mDevice->getCreationParameters();

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)params.ClientWidth)
		{
			if (displayModeList[i].Height == (unsigned int)params.ClientHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;

	/* 如果将这个标志设为DEBUG，则绘制效率大大降低，且帧频极不稳定 */
	createDeviceFlags = 0;

	HRESULT hr;
	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &md3dDevice, &featureLevel, &md3dDeviceContext);

	if (FAILED(hr))
		return false;

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		throw std::runtime_error("DirectX11 is not supported!");
		return false;
	}

	UINT numQualityLevels;
	md3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);

	IDXGIDevice* dxgiDevice = 0;
	md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = params.ClientWidth;
	swapChainDesc.BufferDesc.Height = params.ClientHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (params.VsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set multisampling.
	if (params.MultiSamplingQuality == 0)
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = params.MultiSamplingCount;
		UINT numQualityLevels;
		md3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);

		swapChainDesc.SampleDesc.Quality = min(numQualityLevels - 1, params.MultiSamplingQuality);
	}

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;	

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = (HWND)params.WindowHandle;

	// Set to full screen or windowed mode.
	if (params.WindowStyle & EWS_FULLSCREEN)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	hr = dxgiFactory->CreateSwapChain(md3dDevice, &swapChainDesc, &md3dSwapChain);
	if (FAILED(hr))
		return false;
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);
	// Get the pointer to the back buffer.

	ID3D11Texture2D* backBuffer;
	result = md3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
	{
		return false;
	}
	md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	ReleaseCOM(backBuffer);


	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = params.ClientWidth;
	depthBufferDesc.Height = params.ClientHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;
	depthBufferDesc.SampleDesc.Quality = swapChainDesc.SampleDesc.Quality;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = md3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = md3dDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	md3dDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	md3dDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	md3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Setup the viewport for rendering.
	viewport.Width = static_cast<float>(mDevice->getClientWidth());
	viewport.Height = static_cast<float>(mDevice->getClientHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.static_cast<float>
	md3dDeviceContext->RSSetViewports(1, &viewport);

	//create resource factory
	mResourceFactory = new CD3D11ResourceFactory(md3dDevice, md3dDeviceContext, this);

	//create geometry creator
	mGeometryCreator = new CGeometryCreator;

	//create material manager
	mMaterialManager = new CMaterialManager;

	// create shadermanager
	mShaderManager = new CShaderManager(mResourceFactory);

	// create inputlayout manager
	mInputLayoutManager = new CInputLayoutManager(mResourceFactory);

	//create pipeline manager
	mPipeManager = new CPipelineManager(mResourceFactory);

	//create texture manager
	mTextureManager = new CTextureManager(mResourceFactory);

	//create render state manager
	mRenderStateManager = new CRenderStateManager(mResourceFactory);

	//create mesh manager
	mMeshManager = new CMeshManager(mResourceFactory, mGeometryCreator);

	//create resource group manager
	mResourceGroupManager = new CResourceGroupManager(mTextureManager, mShaderManager,
		mInputLayoutManager, mRenderStateManager, mPipeManager, mMaterialManager, mMeshManager);
	
	return true;
}


CD3D11Driver::~CD3D11Driver()
{
	/* 释放的顺序 
	  1, mesh
	  2, material
	  3, pipeline
	  4, texture
	  5, shader,
	  6, inputlayout
	  7, render-state
	*/
	
	
	ReleaseReferenceCounted(mMeshManager);
	ReleaseReferenceCounted(mMaterialManager);
	ReleaseReferenceCounted(mPipeManager);
	ReleaseReferenceCounted(mTextureManager);
	ReleaseReferenceCounted(mShaderManager);
	ReleaseReferenceCounted(mInputLayoutManager);
	ReleaseReferenceCounted(mRenderStateManager);

	ReleaseReferenceCounted(mResourceFactory);
	ReleaseReferenceCounted(mGeometryCreator);
}

void CD3D11Driver::beginScene(
	bool zBuffer,
	bool stencilBuffer,
	const f32 clearColor[],
	f32 depthValue,
	u8 stencilValue)
{
	md3dDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor);
	
	UINT clearFlag = 0;
	if (zBuffer) clearFlag |= D3D11_CLEAR_DEPTH;
	if (stencilBuffer) clearFlag |= D3D11_CLEAR_STENCIL;

	md3dDeviceContext->ClearDepthStencilView(mDepthStencilView, clearFlag, depthValue, stencilValue);

	D3D11DriverState.Reset();
	md3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CD3D11Driver::endScene()
{
	md3dSwapChain->Present(0, 0);
}


void CD3D11Driver::bindPrimitiveType(E_PRIMITIVE_TYPE primitiveType)
{
	D3D11_PRIMITIVE_TOPOLOGY pt;
	if (primitiveType == EPT_TRIANGLELIST)
		pt = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	md3dDeviceContext->IASetPrimitiveTopology(pt);
}


void CD3D11Driver::setTexture(E_SHADER_TYPE shadertype, u32 slot, ITexture* texture)
{
	CD3D11Texture*	d3d11Texture = dynamic_cast<CD3D11Texture*>(texture);
	if (d3d11Texture->getShaderResourceView() != D3D11DriverState.ShaderResourceViews[shadertype][slot])
	{
		D3D11DriverState.ShaderResourceViews[shadertype][slot] = d3d11Texture->getShaderResourceView();
		D3D11DriverState.ShaderResourceViewIsDirty[shadertype] = true;
	}
}

void CD3D11Driver::bindTexture(E_SHADER_TYPE shaderType, u32 textureCount)
{
	switch (shaderType)
	{
	case EST_VERTEX_SHADER:
		if (D3D11DriverState.ShaderResourceViewIsDirty[EST_VERTEX_SHADER])
		{
			md3dDeviceContext->VSSetShaderResources(0, textureCount, D3D11DriverState.ShaderResourceViews[EST_VERTEX_SHADER]);
			D3D11DriverState.ShaderResourceViewIsDirty[EST_VERTEX_SHADER] = false;
		}
		break;
	case EST_HULL_SHADER:
		if (D3D11DriverState.ShaderResourceViewIsDirty[EST_HULL_SHADER])
		{
			md3dDeviceContext->HSSetShaderResources(0, textureCount, D3D11DriverState.ShaderResourceViews[EST_HULL_SHADER]);
			D3D11DriverState.ShaderResourceViewIsDirty[EST_HULL_SHADER] = false;
		}
		break;
	case EST_DOMAIN_SHADER:
		if (D3D11DriverState.ShaderResourceViewIsDirty[EST_DOMAIN_SHADER])
		{
			md3dDeviceContext->DSSetShaderResources(0, textureCount, D3D11DriverState.ShaderResourceViews[EST_DOMAIN_SHADER]);
			D3D11DriverState.ShaderResourceViewIsDirty[EST_DOMAIN_SHADER] = false;
		}
		break;
	case EST_GEOMETRY_SHADER:
		if (D3D11DriverState.ShaderResourceViewIsDirty[EST_GEOMETRY_SHADER])
		{
			md3dDeviceContext->GSSetShaderResources(0, textureCount, D3D11DriverState.ShaderResourceViews[EST_GEOMETRY_SHADER]);
			D3D11DriverState.ShaderResourceViewIsDirty[EST_GEOMETRY_SHADER] = false;
		}
		break;
	case EST_PIXEL_SHADER:
		if (D3D11DriverState.ShaderResourceViewIsDirty[EST_PIXEL_SHADER])
		{
			md3dDeviceContext->PSSetShaderResources(0, textureCount, D3D11DriverState.ShaderResourceViews[EST_PIXEL_SHADER]);
			D3D11DriverState.ShaderResourceViewIsDirty[EST_PIXEL_SHADER] = false;
		}
		break;
	}
}