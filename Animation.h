#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class Animation
{
private:
	sf::Sprite	m_sprite;						
	size_t		m_keyFrameCount = 1;				// number of frames in anim
	size_t		m_currentFrames = 0;				// total frames so far
	size_t		m_duration		= 0;				// duration of animation
	Vec2		m_size			= { 1.0f, 1.0f };	// size of animation frame
	std::string	m_name			= "none";

public:
	Animation();
	Animation(const std::string& name, const sf::Texture& t);
	Animation(const std::string& name, const sf::Texture& t,
		size_t keyFrameCount, size_t duration);

	sf::Sprite&			getSprite();
	const std::string&	getName() const;
	const Vec2&			getSize() const;
	const int			getCurrentTotalFrames() const;

	void update();
	bool hasEnded() const;
};