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
	size_t		m_totalEntities = 0;
	EntityVec	m_entities;
	EntityVec	m_entitiesToAdd;
	EntityMap	m_entityMap;

	void removeDeadEntities(EntityVec& vec);

public:
	EntityManager();

	void update();

	Entity addEntity(const std::string& tag);

	EntityVec&			getEntities();
	EntityVec&			getEntities(const std::string& tag);
	const EntityMap&	getEntityMap();
};