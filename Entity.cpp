#include "Entity.h"

Entity::Entity()
{}

Entity::Entity(const size_t id)
	: m_id(id)
{}

bool Entity::isAlive() const
{
	return EntityMemoryPool::Instance().isAlive(m_id);
}

const std::string& Entity::tag() const
{
	return EntityMemoryPool::Instance().getTag(m_id);
}

const size_t Entity::id() const
{
	return m_id;
}

void Entity::destroy()
{
	EntityMemoryPool::Instance().destroyEntity(m_id);
}