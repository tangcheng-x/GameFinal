#ifndef __RESOURCE_GROUP_MANAGER_INTERFACE_H__
#define __RESOURCE_GROUP_MANAGER_INTERFACE_H__

#include "IReferenceCounted.h"
#include "IResourceLoader.h"


class IResourceGroupManager : public IReferenceCounted
{
public:
	
	virtual bool init(const std::string& configFileName) = 0;

	virtual bool loadResourceGroup(u32 groupid) = 0;

	virtual bool loadResourceGroup(const std::string& groupname) = 0;

	virtual bool getFullPath(const std::string& resourceFileName, std::string& fullpath) const = 0;

protected:
	
	IResourceLoader*				mResourceLoader;
};


#endif