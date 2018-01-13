//////////////////////////////////
//
// Animation Handler class
// The Animation Handler class handles animation for entities.
// All entities have an Animation Handler class as variable.
//
//////////////////////////////////

#pragma once
#include <SFML\Graphics\Rect.hpp>
#include <vector>

struct Animation
{
	// The amount of frames the animation has minus 1
	const int frameAmount{ 0 };
	// The time an animation should take
	const float frameDuration;
	// Whether the animation should loop after it has been finished
	const bool loop{ true };
	// Whether the animation should reset from the beginning or continue
	// form the same frame when switching from animation
	const bool transition{ false };

	Animation(const int frameAmount, const float frameDuration, const bool loop = true, const bool transition = false);
};

class AnimationHandler
{
private:
	// Used to calculate all frame positions
	const sf::IntRect m_frameSize;
	const sf::IntRect m_spriteSize;

	// Vertical
	int m_animationId{ 0 };
	// Horizontal
	int m_currentFrame{ 0 };
	
	std::vector<Animation> m_animation;
	
	float m_timeline{ 0.f };
	float m_rate{ 1.f };

public:
	// Animations are pushed in the same order as the animations in a sprite sheet are.
	// Use of enumerators is recommended
	void pushAnimation(const Animation &&animation);
	void setAnimation(const int animationId);

	void update(const float elapsedTime);
	// Sets the rate of the timeline, 2 means twice as fast
	void setRate(const float rate);
	void setFrame(int frame);
	void setTimeline(float time);

	sf::IntRect getFrame() const;
	const int getAnimation() const;
	// Used for snapping animations to their latest frame
	const int getLastFrame() const;

	// If sprite size is left uninitialized, frame sizes will be used as hitboxes
	AnimationHandler(const int frameWidth, const int frameHeight, sf::IntRect spriteSize = { 0,0,0,0 });
};

