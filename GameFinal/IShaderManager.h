#ifndef __ISHADERMANAGER_H__
#define __ISHADERMANAGER_H__

#include <map>
#include "IReferenceCounted.h"
#include "IShader.h"

class IShaderManager : public IReferenceCounted
{
public:

	virtual IShader* load(E_SHADER_TYPE shaderType,
		const std::string& filename,
		const std::string& functionName, 
		const std::string& shaderName) = 0;

	virtual IShader* get(const std::string& name) = 0;

	_DECLARE_RESOURCE_DESTROY_METHODS(IShader);

protected:


};

#endif