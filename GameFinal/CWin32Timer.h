#ifndef __CWIN32TIMER_H_INCLUDE__
#define __CWIN32TIMER_H_INCLUDE__

#include "ITimer.h"

class CWin32Timer : public ITimer
{
public:
	CWin32Timer();

	virtual f32 tick();
	virtual f32 getElapseTime() const;

	virtual void start(); // Call when unpaused.
	virtual void stop();  // Call when paused.

	virtual void reset();
private:

	u32			mBaseTime;
	u32			mPausedTime;
	u32			mStopTime;
	u32			mPrevTime;
	u32			mCurrTime;


	bool mStopped;
};

#endif
