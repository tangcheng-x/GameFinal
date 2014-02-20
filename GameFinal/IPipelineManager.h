#ifndef __IPIPELINEMANAGER_H__
#define __IPIPELINEMANAGER_H__

#include "gfTypes.h"
#include "IShader.h"
#include "IPipeline.h"
#include "IInputLayout.h"

class IPipelineManager : public IReferenceCounted
{
public:
	virtual IPipeline* create(const std::string& name, 
		IShader** shaders, 
		u32 shaderCount, 
		IInputLayout* inputlayout,
		E_PRIMITIVE_TYPE primitiveType,
		IRenderState* renderState) = 0;

	virtual IPipeline* get(const std::string& name) = 0;
	
	_DECLARE_RESOURCE_DESTROY_METHODS(IPipeline);
};

#endif

