#include "EntityMemoryPool.h"
#include "Entity.h"


Entity EntityMemoryPool::addEntity(const std::string& tag)
{
	size_t index = getNextFreeIndex();
	// set all vectors[index] = 0
	std::get<std::vector<CTransform>>(m_data)[index] = CTransform();
	std::get<std::vector<CLifespan>>(m_data)[index] = CLifespan();
	std::get<std::vector<CInput>>(m_data)[index] = CInput();
	std::get<std::vector<CBoundingBox>>(m_data)[index] = CBoundingBox();
	std::get<std::vector<CAnimation>>(m_data)[index] = CAnimation();
	std::get<std::vector<CGravity>>(m_data)[index] = CGravity();
	std::get<std::vector<CState>>(m_data)[index] = CState();

	// set tag[index], active[index] = defaults
	m_tags[index] = tag;
	m_active[index] = true;

	m_numEntities++;
	return Entity(index);
}