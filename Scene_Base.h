#pragma once
#include "EntityManager.h"
#include "Scene.h"

class SceneBase : public Scene
{
private:
	struct PlayerConfig
	{
		float X, Y, BBOX_W, BBOX_H, SPEED, MAX_SPEED, JUMP, GRAVITY;
		std::string SHOOT_ANIM;
	};

	std::shared_ptr<Entity> m_player;
	PlayerConfig			m_playerConfig;
	EntityManager			m_entityManager;
	std::string				m_levelPath;
	bool					m_drawTextures = true;
	bool					m_drawCollision = false;
	bool					m_drawGrid = false;
	const Vec2				m_gridSize = { 64, 64 };
	sf::Text				m_gridText;

	void init(const std::string& levelPath);
	void loadLevel(const std::string& levelPath);

	void update() override;
	void onEnd() override;
	void sRender() override;
	void sDoAction(const Action& action) override;

	Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
	void sMovement();
	void sAnimation();
	void sCollision();
	void sLifespan();
	// void setAnimation(std::shared_ptr<Entity> entity, const std::string& animationName, bool repeat);
	void drawLine(const Vec2& p1, const Vec2& p2);

	void initPlayer();
	void spawnBullet(std::shared_ptr<Entity> entity);
	void setAnimation(std::shared_ptr<Entity> entity,
		const std::string& animationName, bool repeat);

public:
	SceneBase(GameEngine* game, const std::string& levelPath);
};