#include "Scene.h"

Scene::Scene()
{}

Scene::Scene(GameEngine* game)
	: m_game(game)
{}

void Scene::togglePause()
{
	m_paused = !m_paused;
}

const ActionMap& Scene::getActionMap() const
{
	return m_actionMap;
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}

void Scene::setPaused(bool paused)
{
	m_paused = paused;
}