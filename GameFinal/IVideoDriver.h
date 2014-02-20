#ifndef IVIDEODRIVER_H
#define IVIDEODRIVER_H

#include "gfTypes.h"
#include "gfUtil.h"
#include "IShaderManager.h"
#include "IInputLayoutManager.h"
#include "IPipelineManager.h"
#include "ITextureManager.h"
#include "IResourceFactory.h"
#include "IMesh.h"
#include "IMaterialManager.h"
#include "IRenderStateManager.h"
#include "IMeshManager.h"
#include "IGeometryCreator.h"
#include "IResourceGroupManager.h"
#include "IShaderVariableInjection.h"

class IVideoDriver : public IReferenceCounted
{
public:
	IVideoDriver()
		:mResourceFactory(nullptr)
		, mShaderManager(nullptr)
		, mInputLayoutManager(nullptr)
		, mPipeManager(nullptr)
		, mTextureManager(nullptr)
		, mRenderStateManager(nullptr)
		, mMeshManager(nullptr)
	{
		
	}

	virtual ~IVideoDriver()
	{

	}
	
	virtual bool init() = 0;

	virtual void beginScene(
		bool backBuffer,
		bool zBuffer, 
		const f32 color[],
		f32 depthValue = 1.0f,
		u8 stencilValue = 0) = 0;
	
	virtual void endScene() = 0;

	const char* getVideoCardDescription() const
	{
		return mVideoCardDescription;
	}

	virtual void bindPrimitiveType(E_PRIMITIVE_TYPE primitiveType) = 0;

	IResourceFactory*		getResourceFactory()
	{
		return mResourceFactory;
	}

	IGeometryCreator* getGeometryCreator()
	{
		return mGeometryCreator;
	}

	IMaterialManager* getMaterialManager()
	{
		return mMaterialManager;
	}

	ITextureManager*		getTextureManager()
	{
		return mTextureManager;
	}

	IRenderStateManager*	getRenderStateManager()
	{
		return mRenderStateManager;
	}

	IMeshManager*			getMeshManager()
	{
		return mMeshManager;
	}

	IShaderManager*			getShaderManager()
	{
		return mShaderManager;
	}

	IInputLayoutManager*	getInputLayoutManager()
	{
		return mInputLayoutManager;
	}

	IPipelineManager*		getPipelineManager()
	{
		return mPipeManager;
	}

	IResourceGroupManager*	getResourceGroupManager()
	{
		return mResourceGroupManager;
	}



protected:
	int								mVideoCardMemory;
	char							mVideoCardDescription[128];
	IResourceFactory*				mResourceFactory;
	IGeometryCreator*				mGeometryCreator;
	IMaterialManager*				mMaterialManager;
	IShaderManager*					mShaderManager;
	IInputLayoutManager*			mInputLayoutManager;
	IPipelineManager*				mPipeManager;
	ITextureManager*				mTextureManager;
	IRenderStateManager*			mRenderStateManager;
	IMeshManager*					mMeshManager;
	IResourceGroupManager*			mResourceGroupManager;
//	IShaderVariableInjection*		mShaderVariableInjector;
};

#endif