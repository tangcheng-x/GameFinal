#ifndef __ITEXTURE_H_INCLUDE__
#define __ITEXTURE_H_INCLUDE__

#include "IReferenceCounted.h"


class ITexture : public IReferenceCounted
{
public:
	ITexture(const std::string& name, u32 sortcode)
		:mName(name), mSortCode(sortcode){}

	const std::string& getName()
	{
		return mName;
	}

	u32 getSortCode() const
	{
		return mSortCode;
	}

	

	virtual ~ITexture() {}
protected:
	std::string		mName;
	u32				mSortCode;
};


#endif