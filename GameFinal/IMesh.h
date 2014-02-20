#ifndef __IMESH_H__
#define __IMESH_H__

#include "IReferenceCounted.h"
#include "EMemoryUsage.h"
#include "IMeshBuffer.h"

enum E_MESH_TYPE
{
	EMT_SIMPLE_MESH = 0,
	EMT_MODEL_MESH,
	EMT_ANIMATE_MODEL_MESH
};

class IMesh : public IReferenceCounted
{
public:
	IMesh(const std::string& name,
		u32 sortcode)
		:mName(name)
		, mSortCode(sortcode)
	{

	}

	virtual E_MESH_TYPE getType() const = 0;

	virtual ~IMesh(){}

	virtual const std::string& getName() const
	{
		return mName;
	}

	u32 getSortCode() const
	{
		return mSortCode;
	}

protected:
	u32					mSortCode;
	const std::string	mName;

};



#endif
