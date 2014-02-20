#ifndef __RENDER_STATE_MANAGER_CLASS_H__
#define __RENDER_STATE_MANAGER_CLASS_H__

#include "IResourceFactory.h"
#include "IRenderStateManager.h"

class CRenderStateManager : public IRenderStateManager
{
public:
	CRenderStateManager(IResourceFactory* pResourceFactory);
	virtual IRenderState* create(const std::string& name);
	virtual IRenderState* get(const std::string& name, bool addIfNotFound = true);

	_DEFINE_RESOURCE_DESTROY_METHODS(CRenderStateManager, mRenderStatesMap, IRenderState);

private:
	IResourceFactory*						mResourceFactory;
	std::map<std::string, IRenderState*>	mRenderStatesMap;
};

#endif
