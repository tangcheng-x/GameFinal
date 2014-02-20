#ifndef __REFERENCE_COUNTED_STRUCT_H__
#define __REFERENCE_COUNTED_STRUCT_H__

#include "gfTypes.h"

class SReferenceCounted
{
public:

	//! Constructor.
	SReferenceCounted()
		:ReferenceCounter(1)
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
			// The difference between IReferenceCounted and SReferenceCounted is 
			// here is no 'delete this' sentence.
			//delete this;
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

