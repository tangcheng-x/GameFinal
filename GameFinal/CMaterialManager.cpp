#include "stdafx.h"
#include "CMaterialManager.h"


CMaterialManager::CMaterialManager()
{

}


CMaterialManager::~CMaterialManager()
{
}

bool CMaterialManager::add(IMaterial& material)
{
	const std::string name = material.getName();
	if (mMaterialsMap.find(name) != mMaterialsMap.end())
	{
		GF_PRINT_CONSOLE_INFO("load the material with the same name (%s).", name.c_str());
		return false;
	}

	u32 sortcode = mCodeAllocator.allocate();
	mMaterialsMap.insert(std::make_pair(name, material));
	return true;
}

IMaterial* CMaterialManager::get(const std::string& name)
{
	auto it = mMaterialsMap.find(name);
	if (it == mMaterialsMap.end())
		return nullptr;

	return &(it->second);
}

IMaterial* CMaterialManager::create(const std::string& name,
	const IMaterial::Material& colors,
	IPipeline* pipelines[],
	u32 pipelineCount)
{
	auto it = mMaterialsMap.find(name);
	if (it != mMaterialsMap.end())
	{
		GF_PRINT_CONSOLE_INFO("The material named '%s' has already been created. \
							  you can't create the materials with the same name twice.",
							  name.c_str());
		return &it->second;
	}

	u32 sortcode = mCodeAllocator.allocate();
	SMaterial material(name, sortcode, colors, pipelines, pipelineCount);

	mMaterialsMap.insert(std::make_pair(name, material));
	it = mMaterialsMap.find(name);
	return &it->second;
}

bool CMaterialManager::destroy(const std::string& name)
{
	auto it = mMaterialsMap.find(name);
	if (it == mMaterialsMap.end())
		return false;

	if (it->second.getReferenceCount() == 1)
	{
		it->second.drop();
		mMaterialsMap.erase(it);
		return true;
	}
	else
		return false;
}

bool CMaterialManager::destroy(IMaterial* material)
{
	const std::string& name = material->getName();
	auto it = mMaterialsMap.find(name);
	if (it == mMaterialsMap.end())
		return false;

	if (it->second.getReferenceCount() == 1)
	{
		it->second.drop();
		mMaterialsMap.erase(it);
		return true;
	}
	else
		return false;
}
