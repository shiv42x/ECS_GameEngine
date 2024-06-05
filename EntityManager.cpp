#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager()
	: m_totalEntities(0)
{}

void EntityManager::update()
{

	// to avoid iterator invalidation
	// entities to add are in m_entitiesToAdd, 
	// then in here, transfer entities from m_entitiesToAdd to m_entities vector
	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e.tag()].push_back(e);
	}
	m_entitiesToAdd.clear();

	// remove dead entities from vector of all entities
	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	//// remove all dead entities from the input vector
	std::erase_if(vec, [](auto& e) { return !e.isAlive(); });
}

Entity EntityManager::addEntity(const std::string& tag)
{
	m_totalEntities++;
	// hack
	auto index = EntityMemoryPool::Instance().addEntity(tag);
	auto addedEntity = Entity(index);
	m_entitiesToAdd.push_back(addedEntity);
	return addedEntity;
}

EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

const EntityMap& EntityManager::getEntityMap()
{
	return m_entityMap;
}