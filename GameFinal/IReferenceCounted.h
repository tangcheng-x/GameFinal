#ifndef __IREFERENCECOUNTED_H__
#define __IREFERENCECOUNTED_H__

#include "gfTypes.h"

class IReferenceCounted
{
public:

	//! Constructor.
	IReferenceCounted()
		:ReferenceCounter(1)
	{

	}

	//! Destructor.
	virtual ~IReferenceCounted()
	{

	}

	
	void grab() const 
	{
		++ReferenceCounter; 
	}

	bool drop() const
	{
		// someone is doing bad reference counting.
		//assert(ReferenceCounter > 0);

		--ReferenceCounter;
		if (!ReferenceCounter)
		{
			delete this;
			return true;
		}

		return false;
	}

	//! Get the reference count.
	/** \return Current value of the reference counter. */
	s32 getReferenceCount() const
	{
		return ReferenceCounter;
	}

private:

	//! The reference counter. Mutable to do reference counting on const objects.
	mutable s32 ReferenceCounter;
};



#endif

