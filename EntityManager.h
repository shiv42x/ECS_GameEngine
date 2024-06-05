#pragma once
#include "Entity.h"
#include <vector>
#include <map>

typedef std::vector<Entity> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class Entity;

class EntityManager
{
private:
	//EntityVec m_entities;
	//EntityMap m_entityMap;
	//EntityVec m_entitiesToAdd;
	size_t    m_totalEntities = 0;
	std::vector<Entity>	m_entities;
	EntityMap m_entityMap;
	std::vector<Entity>	m_entitiesToAdd;

	void removeDeadEntities(std::vector<Entity> vec);

public:
	EntityManager();

	void update();

	Entity addEntity(const std::string& tag);

	std::vector<Entity> getEntities();
	std::vector<Entity> getEntities(const std::string& tag);
	//const std::map<std::string, std::vector<Entity>> & getEntityMap();
};