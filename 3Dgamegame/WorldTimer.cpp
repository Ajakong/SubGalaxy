#include "WorldTimer.h"

namespace
{
	int m_frame = 0;
	int m_minute = 0;
	int m_timeCount = 0;
}

int WorldTimer::GetTimer()
{
	return m_timeCount;
}

int WorldTimer::GetMinute()
{
	return m_minute;
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
		m_timeCount++;
	}
	if (m_timeCount % 60 == 0&&m_timeCount!=0)
	{
		m_timeCount = 0;
		m_minute++;
	}
}

void WorldTimer::Reset()
{
	m_frame = 0;
	m_timeCount = 0;
	m_minute = 0;
}
