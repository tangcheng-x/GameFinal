#ifndef __TEXTURE_MANAGER_CLASS_H__
#define __TEXTURE_MANAGER_CLASS_H__

#include "ITextureManager.h"
#include "IResourceFactory.h"
#include "CSortCodeAllocator.h"

class CTextureManager : public ITextureManager
{
public:
	CTextureManager(IResourceFactory* pResourceFactory);

	virtual ITexture* load(const std::string& name, const std::string& filepath);
	virtual ITexture* get(const std::string& name) const;

	_DEFINE_RESOURCE_DESTROY_METHODS(CTextureManager, mTextureMap, ITexture);
private:
	IResourceFactory*						mResourceFactory;
	std::map<std::string, ITexture*>		mTextureMap;
	CSortCodeAllocator<255>					mCodeAllocator;

};

#endif
