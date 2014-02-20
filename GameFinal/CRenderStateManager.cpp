#include "stdafx.h"
#include "CRenderStateManager.h"
#include "gfUtil.h"

CRenderStateManager::CRenderStateManager(IResourceFactory* pResourceFactory)
:mResourceFactory(pResourceFactory)
{
}

IRenderState* CRenderStateManager::create(const std::string& name)
{
	auto it = mRenderStatesMap.find(name);
	if (it != mRenderStatesMap.end())
	{
		GF_PRINT_CONSOLE_INFO("WARNING: Render State named '%s' have already existed.\n", name.c_str());
		return it->second;
	}

	IRenderState* pRenderState = mResourceFactory->createRenderState(name);
	mRenderStatesMap.insert(std::make_pair(name, pRenderState));
	return pRenderState;
}

IRenderState* CRenderStateManager::get(const std::string& name, bool addIfNotFound)
{
	auto it = mRenderStatesMap.find(name);
	if (it != mRenderStatesMap.end())
	{
		return it->second;
	}

	if (addIfNotFound)
	{
		IRenderState* pRenderState = mResourceFactory->createRenderState(name);
		mRenderStatesMap.insert(std::make_pair(name, pRenderState));
		return pRenderState;
	}

	return nullptr;
}
