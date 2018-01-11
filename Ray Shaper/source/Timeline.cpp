#include "Timeline.h"
#include <cmath>

#include <iostream>

void Timeline::update(const float elapsedTime)
{
	m_timeline += elapsedTime * m_rate;
	if (m_timeline > m_cap)
		m_timeline = m_cap;
	else if (m_timeline < 0)
		m_timeline = 0;
}

void Timeline::setCap(const float cap)
{
	m_cap = cap;
}

void Timeline::setTimeline(const float value)
{
	m_timeline = value;
	if (m_timeline > m_cap)
		m_timeline = m_cap;
}

void Timeline::setRate(const float rate)
{
	m_rate = std::abs(rate);
}

float Timeline::lerp(const float a, const float b)
{
	const float percentage{ getProgress() / 100.f };
	return ((1.f- percentage) * a + (percentage * b));
}

float Timeline::getProgress() const
{
	float progress{ m_timeline * 100.f / m_cap };

	// This happens when cap is 0
	if (progress != progress)
		progress = 100;
	return progress;
}

const float & Timeline::getTimeline() const
{
	return m_timeline;
}
