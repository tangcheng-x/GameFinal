#ifndef __MATERIAL_MANAGER_INTERFACE_H__
#define __MATERIAL_MANAGER_INTERFACE_H__

#include "IReferenceCounted.h"
#include "IMaterial.h"

class IMaterialManager : public IReferenceCounted
{
public:
	virtual IMaterial* create(const std::string& name,
		const IMaterial::Material& colors,
		IPipeline* pipelines[],
		u32 pipelineCount) = 0;

	virtual bool add(IMaterial& material) = 0;
	//virtual IMaterial* load() = 0;
	virtual IMaterial* get(const std::string& name) = 0;

	virtual bool destroy(const std::string& name) = 0;

	virtual bool destroy(IMaterial* material) = 0;
};

#endif
