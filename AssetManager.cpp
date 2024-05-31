#include "AssetManager.h"

#include <fstream>
#include <iostream>

AssetManager::AssetManager()
{}

bool AssetManager::addTexture(const std::string& textureName, const std::string& path,
	bool smooth)
{
	sf::Texture texture;

	if (!texture.loadFromFile(path))
	{
		std::cerr << "(TEXTURE) Failed to " << textureName << ": " << path << std::endl;
		return false;
	}
	texture.setSmooth(smooth);
	m_textureMap[textureName] = texture;
	std::cout << "(TEXTURE) Loaded " << textureName << ": " << path << std::endl;
	return true;
}

bool AssetManager::addTexture(const std::string& textureName, const std::string& path,
	int posX, int posY, int width, int height, bool smooth)
{
	sf::Texture texture;

	if (!texture.loadFromFile(path, sf::IntRect(posX, posY, width, height)))
	{
		std::cerr << "(TEXTURE) Failed to load " << textureName << ": " << path << std::endl;
		return false;
	}
	texture.setSmooth(smooth);
	m_textureMap[textureName] = texture;
	std::cout << "(TEXTURE) Loaded " << textureName << ": " << path << std::endl;
	return true;
}

bool AssetManager::addFont(const std::string& fontName, const std::string& path)
{
	sf::Font font;

	if (!font.loadFromFile(path))
	{
		std::cerr << "(FONT) Failed to load " << fontName << ": " << path << std::endl;
		return false;
	}
	m_fontMap[fontName] = font;
	std::cout << "(FONT) Loaded " << fontName << ": " << path << std::endl;
}

bool AssetManager::addAnimation(const std::string& animationName,
	const std::string& textureName, const size_t keyFramesCount, const size_t duration)
{
	const auto& texture = getTexture(textureName);
	m_animationMap[animationName] = Animation(animationName, texture, keyFramesCount, duration);
	std::cout << "(ANIM) Loaded anim: " << animationName << & std::endl;
	return true;
}

const sf::Texture& AssetManager::getTexture(const std::string& textureName) const
{
	return m_textureMap.at(textureName);
}

const std::map<std::string, sf::Texture>& AssetManager::getTextureMap() const
{
	return m_textureMap;
}

const Animation& AssetManager::getAnimation(const std::string& animationName) const
{
	return m_animationMap.at(animationName);
}

const Animation& AssetManager::_getAnimation(const std::string& animationName)
{
	return m_animationMap.at(animationName);
}

const sf::Font& AssetManager::getFont(const std::string& fontName) const
{
	// TODO: handle not found case
	return m_fontMap.at(fontName);
}

void AssetManager::loadAssetsFromFile(const std::string& path)
{
	std::ifstream fin(path);
	std::string token;
	
	while (fin.good())
	{

		fin >> token; 
		//TOKEN NAME PATH X Y HEIGHT WIDTH SMOOTH
		if (token == "Texture")
		{
			std::string name, path;
			int X, Y, width, height;
			bool smooth;
			fin >> name >> path >> X >> Y >> width >> height >> smooth;
			addTexture(name, path, X, Y, width, height, smooth);
		}
		//TOKEN ANIMNAME TEXTURENAME KEYFRAMESCOUNT DURATION
		else if (token == "Animation")
		{
			std::string name, textureName;
			int keyFramesCount, duration;
			fin >> name >> textureName >> keyFramesCount >> duration;
			addAnimation(name, textureName, keyFramesCount, duration);
		}
		else if (token == "Font")
		{
			std::string name, path;
			fin >> name >> path;
			addFont(name, path);
		}
	}
}