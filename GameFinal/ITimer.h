#ifndef __ITIMER_H_INCLUDE__
#define __ITIMER_H_INCLUDE__

#include "gfTypes.h"
#include "gfUtil.h"
#include "IReferenceCounted.h"

class ITimer : public IReferenceCounted
{
public:
	virtual f32 tick() = 0;
	virtual f32 getElapseTime() const = 0;

	virtual void start() = 0; // Call when unpaused.
	virtual void stop() = 0;  // Call when paused.

	virtual void reset() = 0;
};

#endif
