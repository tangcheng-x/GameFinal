#ifndef __RESOURCE_LOADER_CLASS_H__
#define __RESOURCE_LOADER_CLASS_H__

#include "IResourceLoader.h"
#include "IResourceGroupManager.h"
#include "IShaderManager.h"
#include "IInputLayoutManager.h"
#include "IMaterialManager.h"
#include "IMeshManager.h"
#include "ITextureManager.h"
#include "IRenderStateManager.h"
#include "IPipelineManager.h"


class CResourceLoader : public IResourceLoader
{
public:
	CResourceLoader(IResourceGroupManager* ResourceGroupManager,
		ITextureManager* TextureManager,
		IShaderManager* ShaderManager,
		IInputLayoutManager* InputlayoutManager,
		IRenderStateManager* RenderStateManager,
		IPipelineManager* PipelineManager,
		IMaterialManager* MaterialManager,
		IMeshManager* MeshManager);

	virtual bool loadTexture(const std::string& name);

	virtual bool loadPipelinesFromFile(const std::string& name);

	virtual bool loadMaterialsFromFile(const std::string& name);

	virtual bool loadPipeline(const std::string& fullpath, const IResourceXmlParser::SPipelineCreateParams& createParams) const;

	virtual bool loadMaterial(const std::string& fullpath, const IResourceXmlParser::SMaterialCreateParams& createParams) const;

	virtual bool loadMeshFromFile(const std::string& name);

private:
	IResourceGroupManager*				mResourceGroupManager;
	ITextureManager*					mTextureManager;
	IShaderManager*						mShaderManager;
	IInputLayoutManager*				mInputlayoutManager;
	IRenderStateManager*				mRenderStateManager;
	IPipelineManager*					mPipelineManager;
	IMaterialManager*					mMaterialManager;
	IMeshManager*						mMeshManager;
	IResourceXmlParser*					mResourceXmlParser;
	IModelFileParser*					mModelFileParser;
};


#endif

