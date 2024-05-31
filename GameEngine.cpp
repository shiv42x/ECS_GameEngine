#include "GameEngine.h"
#include "Scene_Base.h"
#include "AssetManager.h"
#include <iostream>

GameEngine::GameEngine()
{
	init();
}

void GameEngine::init()
{
	m_assetManager.loadAssetsFromFile("assets/asset_list.txt");
	m_window.create(sf::VideoMode(1280, 704), "game engine");
	m_window.setFramerateLimit(60);
	changeScene("BASE", std::make_shared<SceneBase>(this, "assets/scene_base.txt"));
}

void GameEngine::run()
{
	while (isRunning())
	{
		update();
	}
}

// drawing happens here
void GameEngine::update()
{
	if (!isRunning()) { return; }
	
	m_window.clear();
	sUserInput();
	currentScene()->update();
	m_window.display();
}

void GameEngine::sUserInput() 
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quit();
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			// check scene action map here.
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
			{
				continue;
			}

			// if key pressed, action type is start, else end
			const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
			currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
		}
	}
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap[m_currentScene];
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (scene)
	{
		m_sceneMap[sceneName] = scene;
	}
	else
	{
		if (m_sceneMap.find(sceneName) == m_sceneMap.end())
		{
			std::cerr << "Warning: Scene does not exist: " << sceneName << std::endl;
			return;
		}
	}
	if (endCurrentScene)
	{
		m_sceneMap.erase(m_sceneMap.find(m_currentScene));
	}

	m_currentScene = sceneName;
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

const AssetManager& GameEngine::getAssetManager() const
{
	return m_assetManager;
}
int GameEngine::getWinWidth()
{
	return m_window.getSize().x;
}

int GameEngine::getWinHeight()
{
	return m_window.getSize().y;
}

bool GameEngine::isRunning() const
{
	return m_running;
}

void GameEngine::quit()
{
	m_running = false;
}