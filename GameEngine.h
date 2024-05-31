#pragma once
#include "SFML/Graphics.hpp"
#include "AssetManager.h"
#include "Scene.h"

#include <map>
#include <memory>

// forward declaration
class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
protected:
	sf::RenderWindow	m_window;
	AssetManager		m_assetManager;
	std::string			m_currentScene;
	SceneMap			m_sceneMap;
	size_t				m_simulationSpeed = 1;
	bool				m_running = true;

	void init();
	void update();

	void sUserInput();
	std::shared_ptr<Scene> currentScene();

public:

	GameEngine();

	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void quit();
	void run();

	int getWinHeight();
	int getWinWidth();

	sf::RenderWindow& window();
	const AssetManager& getAssetManager() const;
	bool isRunning() const;
};