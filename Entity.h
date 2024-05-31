#pragma once
#include "Components.h"

#include <tuple>
#include <string>

class EntityManager;

typedef std::tuple<
	CTransform,
	CLifespan,
	CInput,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState
> ComponentTuple;

class Entity
{
	friend class EntityManager;

	bool			m_alive		= true;
	std::string		m_tag		= "default";
	size_t			m_id		= 0;
	ComponentTuple	m_components;

	Entity(const std::string& tag, const size_t id);

public:
	void					destroy();
	const	size_t			id()		const;
	const	std::string&	tag()		const;
	bool					isAlive()	const;

	// templated functions for diff component types
	template<typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	bool hasComponent() const
	{
		return getComponent<T>().has;
	}

	// variadic template
	// && means it is an rvalue reference
	// ex: entity->addComponent<CTransform>(Vec2(0, 0), Vec2(5, 5));
	template<typename T, typename...TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		// construct component of type T with perfect forwarding
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template<typename T>
	void removeComponent()
	{
		auto& component = getComponent<T>();
		component = T();
		component.has = false;
	}
};