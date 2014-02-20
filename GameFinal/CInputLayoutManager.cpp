#include "stdafx.h"
#include "CInputLayoutManager.h"

IInputLayout* CInputLayoutManager::create(
	const std::vector<SInputLayoutElement>& elements,
	IShader* shader)
{
	u32 hashcode = getHashCode(elements);

	auto it = mInputLayoutsCache.find(hashcode);

	/* if have the input layout with the same hash code,
	just search the layout really equals.*/
	if (it != mInputLayoutsCache.end())
	{
		std::list<IInputLayout*>& layoutList = it->second;
		auto layoutIt = layoutList.begin();
		for (; layoutIt != layoutList.end(); layoutIt++)
		{
			IInputLayout* layout = *layoutIt;
			if (layout->equals(elements))
			{
				return layout;
			}
		}

		//if no equal inputlayout in the list
		u32 sortcode = mCodeAllocator.allocate();
		IInputLayout* inputlayout = mResourceFactory->createInputLayout(sortcode, elements, shader, hashcode);
		if (!inputlayout)
			return nullptr;
		layoutList.push_back(inputlayout);
		return inputlayout;
	}

	// if no input layout with the same hashcode
	u32 sortcode = mCodeAllocator.allocate();
	IInputLayout* inputlayout = mResourceFactory->createInputLayout(sortcode, elements, shader, hashcode);
	if (!inputlayout)
		return nullptr;

	std::list<IInputLayout*> layoutList;
	layoutList.push_back(inputlayout);
	mInputLayoutsCache.insert(std::make_pair(hashcode, layoutList));
	return inputlayout;
}

IInputLayout* CInputLayoutManager::get(const std::vector<SInputLayoutElement>& elements)
{
	int hashcode = getHashCode(elements);
	auto it = mInputLayoutsCache.find(hashcode);

	// if have the input layout with the same hash code
	if (it != mInputLayoutsCache.end())
	{
		auto layoutIt = it->second.begin();
		for (; layoutIt != it->second.end(); layoutIt++)
		{
			IInputLayout* layout = *layoutIt;
			if (layout->equals(elements))
			{
				return layout;
			}
		}
	}

	return nullptr;
}

CInputLayoutManager::~CInputLayoutManager()
{
	destroyAll();
}

void CInputLayoutManager::destroyAll()
{
	for (auto it = mInputLayoutsCache.begin(); it != mInputLayoutsCache.end(); it++)
	{
		std::list<IInputLayout*>& inputLayouts = it->second;
		for (auto it2 = inputLayouts.begin(); it2 != inputLayouts.end(); it2++)
		{
			(*it2)->drop();
		}
	}
}