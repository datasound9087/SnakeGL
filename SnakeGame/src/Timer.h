#pragma once

#define WINDOWS_LEAN_AND_MEAN
#include "Windows.h"

class Timer
{
private:
	LARGE_INTEGER m_Current;
	double m_Frequency;
public:

	Timer()
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		m_Frequency = 1.0 / freq.QuadPart;
		reset();
	}

	void reset()
	{
		QueryPerformanceCounter(&m_Current);
	}

	float timeElapsed()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);

		unsigned __int64 cycles = now.QuadPart - m_Current.QuadPart;
		return (float)(cycles * m_Frequency);

	}

};
