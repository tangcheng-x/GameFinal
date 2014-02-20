#ifndef CD3D11Driver_H
#define CD3D11Driver_H

#include "GameFinal.h"

class CD3D11Driver : public IVideoDriver
{
public:
	struct SD3D11DriverState
	{
		ID3D11Buffer*				VertexBuffers[16];
		ID3D11Buffer*				IndexBuffer;
		IPipeline*					Pipeline;
		IShader*					Shaders[5];
		ID3D11ShaderResourceView*	ShaderResourceViews[5][32];
		bool						ShaderResourceViewIsDirty[5];
		IInputLayout*				InputLayout;
		E_PRIMITIVE_TYPE			PrimitiveType;
		ID3D11DepthStencilState*	DepthStencilState;
		ID3D11RasterizerState*		RasterizerState;
		ID3D11BlendState*			BlendState;

		void Reset()
		{
			memset(VertexBuffers, 0, sizeof(VertexBuffers));
			IndexBuffer = NULL;
			Pipeline = NULL;
			memset(Shaders, 0, sizeof(Shaders));
			memset(ShaderResourceViews, 0, sizeof(ShaderResourceViews));
			memset(ShaderResourceViewIsDirty, 0, sizeof(ShaderResourceViewIsDirty));
			InputLayout = NULL;
			PrimitiveType = EPT_TRIANGLELIST;
			DepthStencilState = NULL;
			RasterizerState = NULL;
			BlendState = NULL;
		}

		SD3D11DriverState()
		{
			Reset();
		}
	};

public:
	CD3D11Driver(IDevice* device);

	bool init();

	virtual void beginScene(
		bool zBuffer,
		bool stencilBuffer,
		const f32 color[],
		f32 depthValue,
		u8 stencilValue);

	virtual void endScene();

	void bindPrimitiveType(E_PRIMITIVE_TYPE primitiveType);

	void setTexture(E_SHADER_TYPE shadertype, u32 slot, ITexture* texture);

	void bindTexture(E_SHADER_TYPE shaderType, u32 textureCount);

	virtual ~CD3D11Driver();

private:
	HWND						mHwnd;
	IDevice*					mDevice;
	IDXGISwapChain*				md3dSwapChain;
	ID3D11Device*				md3dDevice;
	ID3D11DeviceContext*		md3dDeviceContext;
	ID3D11RenderTargetView*		mRenderTargetView;
	ID3D11Texture2D*			mDepthStencilBuffer;
	ID3D11DepthStencilState*	mDepthStencilState;
	ID3D11DepthStencilView*		mDepthStencilView;

public:
	/* this member is to store the current state of d3d11 driver context. */
	SD3D11DriverState			D3D11DriverState; 
};

#endif