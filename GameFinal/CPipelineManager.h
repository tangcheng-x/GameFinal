#ifndef __CPIPELINEMANAGER_H__
#define __CPIPELINEMANAGER_H__

#include "IPipelineManager.h"
#include "IResourceFactory.h"

class CPipelineManager : public IPipelineManager 
{
public:
	CPipelineManager(IResourceFactory* pResourceFactory);

	virtual IPipeline* create(const std::string& name, 
		IShader** shaders, 
		u32 shaderCount, 
		IInputLayout* inputlayout, 
		E_PRIMITIVE_TYPE primitiveType,
		IRenderState* renderState);

	virtual IPipeline* get(const std::string& name);

	_DEFINE_RESOURCE_DESTROY_METHODS(CPipelineManager, mPipelinesCache, IPipeline);

protected:

	std::map<std::string, IPipeline*>	mPipelinesCache;
	IResourceFactory*					mResourceFactory;

};

#endif
