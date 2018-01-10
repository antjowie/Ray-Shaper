//////////////////////////////////
//
// Timeline class
// The Timeline class handles a timeline instance.
// It is used to make timelines easier to handle.
//
//////////////////////////////////

#pragma once

class Timeline
{
private:
	float m_timeline{ 0 };
	float m_cap{ 0 };
	float m_rate{ 1 };

public:
	// Update the timeline value, needs absolute elapsedTime value
	void update(const float elapsedTime);
	// Set the cap of the timeline
	void setCap(const float cap);
	// Set the value of the timeline
	void setTimeline(const float value);
	// Sets the rate of the timeline, 2 means twice as fast
	void setRate(const float rate);
	// Gets a position from a to b
	float lerp(const float a, const float b);

	// Return a percentage [0,100]
	float getProgress() const;
	// Gets a reference to the timeline
	const float &getTimeline() const;
};

