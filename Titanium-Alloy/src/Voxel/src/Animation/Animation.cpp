#include "Animation.h"

#include <iostream>

Voxel::Animation::Animation(Transform original, double maxTime)
	: m_MaxTime(maxTime), m_Time(0.0f), m_Original(original), m_Active(false), m_Index(0)
{
	InsertFrame({ 0.0f, m_Original });
	InsertFrame({ maxTime, m_Original });
}

Voxel::Animation::~Animation()
{

}

void Voxel::Animation::InsertFrame(const AnimationFrame& frame)
{
	for (int i = 0; i < m_Frames.size(); i++)
	{
		const auto& currentFrame = m_Frames.at(i);
		if (currentFrame.time > frame.time)
		{
			m_Frames.insert(m_Frames.begin() + i, frame);
			return;
		}
	}
	m_Frames.push_back(frame);
}

void Voxel::Animation::UpdateTime(double timeElapsed)
{
	if (m_Active)
	{
		m_Time += timeElapsed;
	}

	while (m_Time >= m_MaxTime && m_MaxTime > 0.0f)
	{
		m_Time -= m_MaxTime;
	}
}

Voxel::Transform Voxel::Animation::GetTransform()
{
	if (m_Frames.size() == 0)
	{
		return m_Original;
	}

	unsigned int currentIndex = 0;
	for (int i = 0; i < m_Frames.size(); i++)
	{
		if (m_Frames.at(i).time >= m_Time)
		{
			break;
		}

		currentIndex++;
	}

	AnimationFrame a1;
	AnimationFrame a2;

	if (currentIndex >= m_Frames.size())
	{
		a1 = m_Frames.at(m_Frames.size() - 1);
		AnimationFrame temp = m_Frames.at(0);
		a2 = { m_MaxTime + temp.time + 0.01f, temp.t };
	}
	else if (currentIndex == 0)
	{
		a1 = { 0.0f, m_Original };
		a2 = m_Frames.at(0);
	}
	else
	{
		a1 = m_Frames.at(currentIndex - 1);
		a2 = m_Frames.at(currentIndex);
	}

	float elapsed;
	if (a2.time == a1.time)
	{
		elapsed = 0.1f;
	}
	else
	{
		elapsed = (a2.time - m_Time) / (a2.time - a1.time);
	}

	return a1.t.Interpolate(a2.t, elapsed);
}

void Voxel::Animation::Play()
{
	m_Active = true;
}

void Voxel::Animation::Pause()
{
	m_Active = false;
}

void Voxel::Animation::Reset()
{
	m_Time = 0;
}
