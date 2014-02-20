#ifndef __RESOURCE_LOADER_INTERFACE_H__
#define __RESOURCE_LOADER_INTERFACE_H__

#include "IReferenceCounted.h"
#include "IShaderManager.h"
#include "IResourceXmlParser.h"
#include "IModelFileParser.h"

class IResourceLoader : public IReferenceCounted
{
public:
	virtual bool loadTexture(const std::string& name) = 0;

	virtual bool loadPipelinesFromFile(const std::string& name) = 0;

	virtual bool loadMaterialsFromFile(const std::string& name) = 0;

	virtual bool loadMeshFromFile(const std::string& name) = 0;
};

#endif
