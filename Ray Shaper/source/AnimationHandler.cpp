#include "AnimationHandler.h"
#include <iostream>
Animation::Animation(const  int frameAmount, const float frameDuration, const bool loop, const  bool transition):
	frameAmount(frameAmount - 1),
	frameDuration(frameDuration),
	loop(loop),
	transition(transition)
{
}

void AnimationHandler::pushAnimation(const Animation && animation)
{
	m_animation.push_back(std::move(animation));
}

void AnimationHandler::setAnimation(const int animationId)
{
	if (static_cast<size_t>(animationId) > m_animation.size() || animationId == m_animationId)
		return;
	m_animationId = animationId;
	
	if (m_animation[m_animationId].transition)
	{
		if (m_currentFrame > m_animation[m_animationId].frameAmount)
			m_currentFrame = m_animation[m_animationId].frameAmount;
		return;
	}
	m_currentFrame = 0;
}

void AnimationHandler::update(const float elapsedTime)
{
	m_timeline += elapsedTime * m_rate;
	
	if (m_timeline > m_animation[m_animationId].frameDuration)
	{
		if (m_animation[m_animationId].loop)
			m_timeline = std::fmod(m_timeline, m_animation[m_animationId].frameDuration * (m_animation[m_animationId].frameAmount + 1));
		else if (m_timeline > m_animation[m_animationId].frameDuration * (m_animation[m_animationId].frameAmount + 1))
				m_timeline = m_animation[m_animationId].frameDuration * (m_animation[m_animationId].frameAmount);
		m_currentFrame = static_cast<int>(m_timeline / m_animation[m_animationId].frameDuration);
	}
}

void AnimationHandler::setRate(const float rate)
{
	m_rate = rate;
}

void AnimationHandler::setFrame(int frame)
{
	frame--;
	if (frame < 0)
		frame = 0;
	else if (frame > m_animation[m_animationId].frameAmount)
		frame = m_animation[m_animationId].frameAmount;
	m_currentFrame = frame;
}

void AnimationHandler::setTimeline(float time)
{
	m_timeline = time;
}

sf::IntRect AnimationHandler::getFrame() const
{
	return sf::IntRect(m_frameSize.width * m_currentFrame + m_spriteSize.left, 
		m_frameSize.height * m_animationId + m_spriteSize.top,m_spriteSize.width,m_spriteSize.height);
}

const int AnimationHandler::getAnimation() const
{
	return m_animationId;
}

const int AnimationHandler::getLastFrame() const
{
	return m_animation[m_animationId].frameAmount;
}

AnimationHandler::AnimationHandler(const int frameWidth, const int frameHeight, sf::IntRect spriteSize):
	m_frameSize(sf::IntRect(1,1,frameWidth,frameHeight)),
	m_spriteSize(spriteSize)
{
	if (spriteSize == sf::IntRect(0, 0, 0, 0))
	{
		spriteSize.width = m_frameSize.width;
		spriteSize.height = m_frameSize.height;
	}
}