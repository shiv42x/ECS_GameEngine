#pragma once
#include "Animation.h"

#include <map>

class AssetManager
{
private:
	std::map<std::string, sf::Texture>	m_textureMap;
	std::map<std::string, Animation>	m_animationMap;
	std::map<std::string, sf::Font>		m_fontMap;

	bool addTexture(const std::string& textureName, const std::string& path,
		bool smooth = true);
	bool addTexture(const std::string& textureName, const std::string& path,
		int posX, int posY, int width, int height,
		bool smooth = true);
	bool addFont(const std::string& textureName, const std::string& path);
	bool addAnimation(const std::string& animationName,
		const std::string& textureName, const size_t frameCount,
		const size_t duration);

public:
	AssetManager();

	const sf::Texture& getTexture(const std::string& textureName) const;
	const Animation& getAnimation(const std::string& animationName) const;
	const Animation& _getAnimation(const std::string& animationName);
	const sf::Font& getFont(const std::string& fontName) const;
	const std::map<std::string, sf::Texture>& getTextureMap() const;

	void loadAssetsFromFile(const std::string& filePath);
};