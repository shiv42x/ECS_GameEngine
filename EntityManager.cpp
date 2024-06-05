#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager()
	: m_totalEntities(0)
{}

void EntityManager::update()
{

	//// to avoid iterator invalidation
	//// entities to add are in m_entitiesToAdd, 
	//// then in here, transfer entities from m_entitiesToAdd to m_entities vector
	//for (auto& e : m_entitiesToAdd)
	//{
	//	m_entities.push_back(e);
	//	m_entityMap[e->tag()].push_back(e);
	//}
	//m_entitiesToAdd.clear();

	//// remove dead entities from vector of all entities
	//removeDeadEntities(m_entities);

	//// remove dead entities from each vector in the entity map
	//for (auto& [tag, entityVec] : m_entityMap)
	//{
	//	removeDeadEntities(entityVec);
	//}
}

//void EntityManager::removeDeadEntities(EntityVec& vec)
//{
//	//// remove all dead entities from the input vector
//	//std::erase_if(vec, [](auto& e) { return !e->isAlive(); });
//}

Entity EntityManager::addEntity(const std::string& tag)
{
	std::cout << "Adding entity!" << std::endl;

	m_totalEntities++;
	// hack
	auto index = EntityMemoryPool::Instance().addEntity(tag);
	auto addedEntity = Entity(index);
	std::cout << "Pushing entity to m_entities!" << std::endl;

	m_entities.push_back(addedEntity);
	std::cout << "Adding to entity map!" << std::endl;

	m_entityMap[addedEntity.tag()].push_back(addedEntity);
	std::cout << "Added entity!" << std::endl;
	return addedEntity;
}

std::vector<Entity> EntityManager::getEntities()
{
	return m_entities;
}

std::vector<Entity> EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

//const std::map<std::string, EntityVec>& EntityManager::getEntityMap()
//{
//	return m_entityMap;
//}