#include "WorldTimer.h"

namespace
{
	int m_frame = 0;
	int mm_timeCount = 0;
}

int WorldTimer::GetTimer()
{
	return mm_timeCount;
}

int WorldTimer::GetFrame()
{
	return m_frame;
}

void WorldTimer::Update()
{
	m_frame++;
	if (m_frame % 60 == 0)
	{
		mm_timeCount++;
	}
}

void WorldTimer::Reset()
{
	m_frame = 0;
	mm_timeCount = 0;
}
