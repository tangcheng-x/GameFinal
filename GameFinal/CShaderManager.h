#ifndef __CD3D11SHADERMANAGER_H__
#define __CD3D11SHADERMANAGER_H__

#include "IShaderManager.h"
#include "IResourceFactory.h"
#include "CSortCodeAllocator.h"

const int MAX_SHADER_SORT_CODE = 255;

class CShaderManager : public IShaderManager
{
public:
	CShaderManager(IResourceFactory* pResourceFactory);
	
protected:

	virtual IShader* load(E_SHADER_TYPE shaderType,
		const std::string& filename,
		const std::string& functionName,
		const std::string& shaderName);


	virtual IShader* get(const std::string& name);

	_DEFINE_RESOURCE_DESTROY_METHODS(CShaderManager, mShadersMap, IShader);

private:
	IResourceFactory*								mResourceFactory;

	std::map<std::string, IShader*>					mShadersMap;

	CSortCodeAllocator<MAX_SHADER_SORT_CODE>		mCodeAllocator[5];

};

#endif