#ifndef __TEXTURE_MANAGER_INTERFACE_H__
#define __TEXTURE_MANAGER_INTERFACE_H__

#include "IResourceFactory.h"

class ITextureManager : public IReferenceCounted
{
public:
	virtual ITexture* load(const std::string& name, const std::string& filepath) = 0;
	virtual ITexture* get(const std::string& name) const = 0;

	_DECLARE_RESOURCE_DESTROY_METHODS(ITexture);

};


#endif

