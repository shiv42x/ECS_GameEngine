#pragma once
#include "Components.h"
#include "EntityMemoryPool.h"
#include <string>


class EntityManager;

class Entity
{
	friend class EntityManager;
	friend class EntityMemoryPool;

	size_t			m_id		= 0;
	Entity(const size_t id);

public:
	// templated functions for diff component types
	Entity();
	bool isAlive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();

	template<typename T, typename...TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = EntityMemoryPool::Instance().getComponent<T>(m_id);
		// construct component of type T with perfect forwarding
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template<typename T>
	T& getComponent()
	{
		return EntityMemoryPool::Instance().getComponent<T>(m_id);
	}

	template<typename T>
	bool hasComponent()
	{
		return EntityMemoryPool::Instance().hasComponent<T>(m_id);
	}

	template <typename T>
	void removeComponent(size_t entityId)
	{
		EntityMemoryPool::Instance().removeComponent(entityId);
	}
};