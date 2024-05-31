#include "Animation.h"

Animation::Animation()
{}

Animation::Animation(const std::string& name, const sf::Texture& t)
	: Animation(name, t, 1, 0)
{}
	
Animation::Animation(const std::string& name, const sf::Texture& t, size_t keyFrameCount,
	size_t duration)
	:	m_name(name)
	,	m_sprite(t)
	,	m_keyFrameCount(keyFrameCount)
	,	m_currentFrames(0)
	,	m_duration(duration)
{
	m_size = Vec2((float)t.getSize().x / keyFrameCount, (float)t.getSize().y);	
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrames) * m_size.x, 0, m_size.x, m_size.y));
}

void Animation::update()
{
	m_currentFrames++;
	int animFrame = (m_currentFrames / m_duration) % m_keyFrameCount; 
	sf::IntRect rect = sf::IntRect(animFrame * m_size.x, 0, m_size.x, m_size.y);
	m_sprite.setTextureRect(rect);
}

const int Animation::getCurrentTotalFrames() const
{
	return m_currentFrames;
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}

const std::string& Animation::getName() const
{
	return m_name;
}

const Vec2& Animation::getSize() const
{
	return m_size;
}

bool Animation::hasEnded() const
{
	int animFrame = (m_currentFrames / m_duration) % m_keyFrameCount;
	return (m_currentFrames > m_duration && animFrame == 0) ? true : false;
}