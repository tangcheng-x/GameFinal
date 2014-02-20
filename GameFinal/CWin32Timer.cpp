#include "stdafx.h"
#include "CWin32Timer.h"
#include <mmsystem.h>

CWin32Timer::CWin32Timer()
: mBaseTime(0)
, mPausedTime(0)
, mPrevTime(0)
, mCurrTime(0)
, mStopped(false)
{

}


void CWin32Timer::reset()
{
	mCurrTime = timeGetTime();

	mBaseTime = mCurrTime;
	mPrevTime = mCurrTime;
	mStopTime = 0;
	mStopped = false;
}

// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
f32 CWin32Timer::getElapseTime() const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// mStopTime - mBaseTime includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

	if (mStopped)
	{
		return (f32)(((mStopTime - mPausedTime) - mBaseTime) * 0.001f);
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime

	else
	{
		return (f32)(((mCurrTime - mPausedTime) - mBaseTime) * 0.001f);
	}
}

f32 CWin32Timer::tick()
{
	if (mStopped)
	{
		return 0.0f;
	}



	mCurrTime = timeGetTime();
	// Time difference between this frame and the previous.
	f32 deltaTime = ((mCurrTime - mPrevTime) * 0.001f);

	// Prepare for next frame.
	mPrevTime = mCurrTime;
	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (deltaTime < 0.0)
	{
		deltaTime = 0.0;
	}

	return deltaTime;
}

void CWin32Timer::start()
{

	u32 startTime = timeGetTime();
	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	if (mStopped)
	{
		mPausedTime += (startTime - mStopTime);

		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void CWin32Timer::stop()
{
	if (!mStopped)
	{
		u32 currTime = timeGetTime();

		mStopTime = currTime;
		mStopped = true;
	}
}
