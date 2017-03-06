#include "Timer.h"
#include <Windows.h>
#include <iostream>

using namespace IceDogCore;

Timer::Timer() :r_currentFPS(0), r_fpsTempCalled(0), r_fpsTemp(0)
{
	__int64 countPersecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPersecond);
	r_secondPerCount = 1.0 / (double)countPersecond;
	QueryPerformanceCounter((LARGE_INTEGER*)&r_prevTime);
}

void Timer::Tick()
{
	// get current count
	__int64 currentCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);
	// cal the delta time
	r_deltaTime = (currentCount - r_prevTime)*r_secondPerCount;
	if (r_deltaTime < 0) { r_deltaTime = 0.0; }
	// update the preTime
	r_prevTime = currentCount;

	r_fpsTempCalled++;
	r_fpsTemp += r_deltaTime;
	if (r_fpsTemp > 1)
	{
		r_fpsTemp = 0;
		r_currentFPS = r_fpsTempCalled;
		r_fpsTempCalled = 0;
	}
}

int IceDogCore::Timer::GetSleepMS(int lockFPS)
{
	return r_deltaTime *1000*60;
}
