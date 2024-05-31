#pragma once
#include "GameEngine.h"
#include "EntityManager.h"
#include "Action.h"

#include <map>
#include <memory>

// forward declaration
class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{
protected:
	GameEngine*		m_game = nullptr;
	EntityManager	m_entityManager;
	ActionMap		m_actionMap;
	sf::Font		m_sceneFont;
	bool			m_paused = false;
	bool			m_hasEnded = false;
	size_t			m_currentFrame = 0;

	void togglePause();

public:
	Scene();
	Scene(GameEngine* game);

	virtual void update() = 0;
	virtual void onEnd() = 0;
	virtual void sRender() = 0;
	virtual void sDoAction(const Action& action) = 0;

	const ActionMap& getActionMap() const;

	void registerAction(int inputKey, const std::string& actionName);
	void setPaused(bool paused);
};