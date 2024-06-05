#include <vector>
#include <map>

#define MAX_ENTITIES 50000

typedef std::tuple<
	std::vector<CTransform>,
	std::vector<CLifespan>,
	std::vector<CInput>,
	std::vector<CBoundingBox>,
	std::vector<CAnimation>,
	std::vector<CGravity>,
	std::vector<CState>
> EntityComponentVectorTuple;

class EntityMemoryPool
{
	size_t						m_numEntities;
	EntityComponentVectorTuple	m_data;
	std::vector<std::string>	m_tags;
	std::vector<bool>			m_active;
	EntityMemoryPool(size_t maxEntities) : m_numEntities(0)
	{
		// Reserve space for vectors in EntityComponentVectorTuple
		std::get<std::vector<CTransform>>(m_data).reserve(maxEntities); // CTransform
		std::get<std::vector<CLifespan>>(m_data).reserve(maxEntities); // CLifespan
		std::get<std::vector<CInput>>(m_data).reserve(maxEntities); // CInput
		std::get<std::vector<CBoundingBox>>(m_data).reserve(maxEntities); // CBoundingBox
		std::get<std::vector<CAnimation>>(m_data).reserve(maxEntities); // CAnimation
		std::get<std::vector<CGravity>>(m_data).reserve(maxEntities); // CGravity
		std::get<std::vector<CState>>(m_data).reserve(maxEntities); // CState

		// Reserve space for tags and active flags
		m_tags.reserve(maxEntities);
		m_active.reserve(maxEntities);

		std::cout << sizeof(EntityComponentVectorTuple) << std::endl;
	}

public:
	static EntityMemoryPool& Instance()
	{
		static EntityMemoryPool pool(MAX_ENTITIES);
		return pool;
	}

	std::vector<std::string> getTags() const
	{
		return m_tags;
	}

	size_t getNextFreeIndex()
	{
		for (size_t i = 0; i < m_active.size(); i++)
		{
			if (!m_active[i])
			{
				return i;
			}
		}
	}

	size_t addEntity(const std::string& tag)
	{
		size_t index = getNextFreeIndex();
		if (index < std::get<std::vector<CTransform>>(m_data).size())
		{
			std::cout << "Found empty space in vec!" << std::endl;
			std::get<std::vector<CTransform>>(m_data)[index] = CTransform();
			std::get<std::vector<CLifespan>>(m_data)[index] = CLifespan();
			std::get<std::vector<CInput>>(m_data)[index] = CInput();
			std::get<std::vector<CBoundingBox>>(m_data)[index] = CBoundingBox();
			std::get<std::vector<CAnimation>>(m_data)[index] = CAnimation();
			std::get<std::vector<CGravity>>(m_data)[index] = CGravity();
			std::get<std::vector<CState>>(m_data)[index] = CState();
		
			m_tags[index] = tag;
			m_active[index] = true;

			m_numEntities++;
			return index;
		}
		std::cout << "Found no empty space in vec!" << std::endl;
		std::get<std::vector<CTransform>>(m_data).push_back(CTransform());
		std::get<std::vector<CLifespan>>(m_data).push_back(CLifespan());
		std::get<std::vector<CInput>>(m_data).push_back(CInput());
		std::get<std::vector<CBoundingBox>>(m_data).push_back(CBoundingBox());
		std::get<std::vector<CAnimation>>(m_data).push_back(CAnimation());
		std::get<std::vector<CGravity>>(m_data).push_back(CGravity());
		std::get<std::vector<CState>>(m_data).push_back(CState());

		m_tags.push_back(tag);
		m_active.push_back(true);
		
		m_numEntities++;
		return index;
	}

	bool isAlive(size_t entityId) 
	{
		return m_active[entityId];
	}

	void destroyEntity(size_t entityId)
	{
		m_numEntities--;
		m_active[entityId] = false;
	}

	const std::string& getTag(size_t entityId) const
	{
		return m_tags[entityId];
	}

	template <typename T>
	T& getComponent(size_t entityId)
	{
		return std::get<std::vector<T>>(m_data)[entityId];
	}

	template <typename T>
	bool hasComponent(size_t entityId)
	{
		return std::get<std::vector<T>>(m_data)[entityId].has;
	}

	template <typename T>
	void removeComponent(size_t entityId)
	{
		auto& component = getComponent<T>(entityId);
		component.has = false;
	}
};