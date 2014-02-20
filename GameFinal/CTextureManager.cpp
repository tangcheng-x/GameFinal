#include "stdafx.h"
#include "gfUtil.h"
#include "CTextureManager.h"


CTextureManager::CTextureManager(IResourceFactory* pResourceFactory)
:mResourceFactory(pResourceFactory)
{
	
}

ITexture* CTextureManager::load(const std::string& name, const std::string& filepath)
{
	auto it = mTextureMap.find(name);
	if (it != mTextureMap.end())
	{
		GF_PRINT_CONSOLE_INFO("Texture named %s has already been loaded.\n", name.c_str());
		return it->second;
	}

	u32 sortcode = mCodeAllocator.allocate();
	ITexture* pTexture = mResourceFactory->loadTextureFromFile(name, filepath, sortcode);
	if (pTexture == nullptr)
		mCodeAllocator.release(sortcode);

	mTextureMap.insert(std::make_pair(name, pTexture));
	return pTexture;
}

ITexture* CTextureManager::get(const std::string& name) const
{
	auto it = mTextureMap.find(name);
	if (it == mTextureMap.end())
		return nullptr;

	return it->second;
}


