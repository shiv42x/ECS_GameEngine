#include "Scene_Base.h"
#include "Physics.h"
#include <fstream>
#include <iostream>

SceneBase::SceneBase(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath)
{
	init(levelPath);
}

void SceneBase::init(const std::string& levelPath)
{
	m_entityManager = EntityManager();

	std::ifstream fin(levelPath);
	std::string token;

	while (fin.good())
	{
		fin >> token;
		if (token == "Player")
		{
			// init player
			std::string ANIM_NAME, SHOOT_ANIM;
			float GRID_X, GRID_Y, BBOX_W, BBOX_H, SPEED, JMP_SPEED, MAX_SPEED, GRAVITY;
			fin >> ANIM_NAME >> GRID_X >> GRID_Y >> BBOX_W >> BBOX_H >> SPEED >> JMP_SPEED >> MAX_SPEED >> GRAVITY >> SHOOT_ANIM;
		
			// X, Y, BBOX_W, BBOX_H, SPEED, MAX_SPEED, JUMP, GRAVITY
			m_playerConfig = { GRID_X, GRID_Y, BBOX_W, BBOX_H, SPEED, MAX_SPEED, JMP_SPEED, GRAVITY, SHOOT_ANIM };
			initPlayer();
		}
		else
		{
			std::string animName;
			float X, Y;
			fin >> animName >> X >> Y;
			auto entity = m_entityManager.addEntity(token);
			// must give animation first, so gridToMidPixel can determine correct size
			entity.addComponent<CAnimation>(m_game->getAssetManager().getAnimation(animName), true);
			entity.addComponent<CTransform>(gridToMidPixel(X, Y, entity));
			
			// only give tiles collision
			if (token == "Tile")
			{
				entity.addComponent<CBoundingBox>(m_game->getAssetManager().getAnimation(animName).getSize());
			}
		}
	}

	// player actions
	registerAction(sf::Keyboard::W, "JUMP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::F, "SHOOT");


	// ui actions
	registerAction(sf::Keyboard::L, "TOGGLE_GRID");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURES");
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION_BOXES");

	m_sceneFont.loadFromFile("assets/fonts/cour.ttf");
	m_gridText.setFont(m_sceneFont);
	m_gridText.setCharacterSize(12);
}

void SceneBase::update()
{
	if (!m_paused)
	{
		m_entityManager.update();
		sMovement();
		sCollision();
		sAnimation();
		sRender();
	}
}

void SceneBase::initPlayer()
{
	// should read playerConfig struct after reading from file
	auto entity = m_entityManager.addEntity("Player");
	entity.addComponent<CAnimation>(m_game->getAssetManager().getAnimation("P_IDLE"), true);
	entity.addComponent<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, entity), Vec2(0.0f, 0.0f), Vec2(-2.5f, 2.5f), 0.0f);
	entity.addComponent<CState>("P_IDLE");
	entity.addComponent<CBoundingBox>(Vec2(m_playerConfig.BBOX_W, m_playerConfig.BBOX_H));
	entity.addComponent<CGravity>(m_playerConfig.GRAVITY);
	m_player = entity;
}

void SceneBase::sAnimation()
{
	// only state changes of player are handled
	const auto& playerState = m_player.getComponent<CState>().state;
	if (playerState == "P_IDLE" && m_player.getComponent<CAnimation>().animation.getName() != "P_IDLE")
	{
		m_player.addComponent<CAnimation>(m_game->getAssetManager().getAnimation("P_IDLE"), false);
	}
	else if (playerState == "P_RUN" && m_player.getComponent<CAnimation>().animation.getName() != "P_RUN")
	{
		m_player.addComponent<CAnimation>(m_game->getAssetManager().getAnimation("P_RUN"), false);
	}
	else if (playerState == "P_JUMP" && m_player.getComponent<CAnimation>().animation.getName() != "P_JUMP")
	{
		m_player.addComponent<CAnimation>(m_game->getAssetManager().getAnimation("P_JUMP"), false);
	}

	for (auto& e : m_entityManager.getEntities())
	{
		if (e.hasComponent<CAnimation>())
		{
			e.getComponent<CAnimation>().animation.update();
		}
	}
}

void SceneBase::sMovement()
{
	Vec2& playerVelocity = m_player.getComponent<CTransform>().velocity;
	auto& input = m_player.getComponent<CInput>();
	auto& playerState = m_player.getComponent<CState>().state;

	// vertical movement
	if (input.up && input.canJump)
	{
		playerVelocity.y = m_playerConfig.JUMP;
		input.canJump = false;
	}
	if (!input.up && playerVelocity.y <= 0)
	{
		playerVelocity.y += m_player.getComponent<CGravity>().gravity / 4;
	}

	// horizontal movement
	if (input.left)
	{
		playerVelocity.x = -m_playerConfig.SPEED;
	}
	if (input.right)
	{
		playerVelocity.x = m_playerConfig.SPEED;
	}
	if (!input.left && !input.right)
	{
		playerVelocity.x = 0.0f;
	}

	// only do player for now
	for (auto& e : m_entityManager.getEntities("Player"))
	{
		playerVelocity.y += m_player.getComponent<CGravity>().gravity;
		playerVelocity.y = std::min(playerVelocity.y, m_playerConfig.MAX_SPEED);

		e.getComponent<CTransform>().prevPos = e.getComponent<CTransform>().pos;
		e.getComponent<CTransform>().pos += e.getComponent<CTransform>().velocity;
	}

	for (auto& e : m_entityManager.getEntities("Bullet"))
	{
		e.getComponent<CTransform>().velocity.y += e.getComponent<CGravity>().gravity;
		e.getComponent<CTransform>().velocity.y = std::min(e.getComponent<CTransform>().velocity.y, m_playerConfig.MAX_SPEED);

		e.getComponent<CTransform>().prevPos = e.getComponent<CTransform>().pos;
		e.getComponent<CTransform>().pos += e.getComponent<CTransform>().velocity;
	}

	// make player face correct direction
	if (playerVelocity.x < 0)
	{
		m_player.getComponent<CTransform>().scale = { 2.5f, 2.5f };
	}
	else if (playerVelocity.x > 0)
	{
		m_player.getComponent<CTransform>().scale = { -2.5f, 2.5f };
	}
}

void SceneBase::sCollision()
{
	// simple aabb
	for (auto& e : m_entityManager.getEntities("Tile"))
	{
		Vec2 overlap = Physics::GetOverlap(m_player, e);
		Vec2 prevOverlap = Physics::GetPreviousOverlap(m_player, e);
		auto& playerVelocity = m_player.getComponent<CTransform>().velocity;
		auto& playerPos = m_player.getComponent<CTransform>().pos;
		auto& playerPrevPos = m_player.getComponent<CTransform>().prevPos;
		auto& entityPos = e.getComponent<CTransform>().pos;

		auto& playerState = m_player.getComponent<CState>().state;

		if (overlap.x > 0 && overlap.y > 0)
		{
			// horizontal collision if prevOverlap.y > 0
			if (prevOverlap.y > 0)
			{
				// if moving right, push to left and vice versa
				// collision when player moved right
				if (playerPrevPos.x < entityPos.x)
				{
					playerPos.x -= overlap.x;
				}
				// collision when player moved down
				else if (playerPrevPos.x > entityPos.x)
				{
					playerPos.x += overlap.x;
				}
			}

			// vertical collision if prevOverlap.x > 0
			if (prevOverlap.x > 0)
			{
				// collision when player moved up
				if (playerPrevPos.y > entityPos.y)
				{
					playerPos.y += overlap.y;
					m_player.getComponent<CTransform>().velocity.y = 0.0f;
				}
				// collision when player moved down
				else if (playerPrevPos.y < entityPos.y)
				{
					playerPos.y -= overlap.y;
					m_player.getComponent<CInput>().canJump = true;
					m_player.getComponent<CTransform>().velocity.y = 0.0f;
				}
			}
		}

		// state machine
		// does this belong here? but sCollision is the last place player physics is modified
		if (playerVelocity.y != 0)
		{
			playerState = "P_JUMP";
			m_player.getComponent<CInput>().canJump = false;
		}
		else if (playerVelocity.x != 0 && playerVelocity.y == 0)
		{
			playerState = "P_RUN";
		}
		else
		{
			playerState = "P_IDLE";
		}
	}

	for (auto& bullet : m_entityManager.getEntities("Bullet"))
	{
		for (auto& tile : m_entityManager.getEntities("Tile"))
		{
			Vec2 overlap = Physics::GetOverlap(bullet, tile);
			Vec2 prevOverlap = Physics::GetPreviousOverlap(bullet, tile);
			auto& bulletVelo = bullet.getComponent<CTransform>().velocity;
			auto& bulletPos = bullet.getComponent<CTransform>().pos;
			auto& bulletPrevPos = bullet.getComponent<CTransform>().prevPos;
			auto& tilePos = tile.getComponent<CTransform>().pos;

			if (overlap.x > 0 && overlap.y > 0)
			{
				// horizontal collision if prevOverlap.y > 0
				if (prevOverlap.y > 0)
				{
					// if moving right, push to left and vice versa
					// collision when player moved right
					if (bulletPrevPos.x < tilePos.x)
					{
						bulletPos.x -= overlap.x;
					}
					else if (bulletPrevPos.x > tilePos.x)
					{
						bulletPrevPos.x += overlap.x;
					}
					// simulate friction
					bullet.getComponent<CTransform>().velocity.x = -bullet.getComponent<CTransform>().velocity.x * 0.9f;

				}

				if (prevOverlap.x > 0)
				{
					if (bulletPrevPos.y > tilePos.y)
					{
						bulletPos.y += overlap.y;
						bullet.getComponent<CTransform>().velocity.y = 0.0f;
					}
					else if (bulletPrevPos.y < tilePos.y)
					{
						bulletPos.y -= overlap.y;
						bullet.getComponent<CTransform>().velocity.y = -bullet.getComponent<CTransform>().velocity.y * 0.5f;
					}
					// simulate friction
					bullet.getComponent<CTransform>().velocity.x = bullet.getComponent<CTransform>().velocity.x * 0.75f;

				}
			}
		}
	}

}

void SceneBase::sRender()
{
	if (!m_paused)
	{
		m_game->window().clear(sf::Color(92, 148, 252));
	}
	else
	{
		m_game->window().clear(sf::Color(4, 89, 247));
	}

	if (m_drawTextures)
	{

		for (auto& e : m_entityManager.getEntities())
		{
			if (e.hasComponent<CAnimation>())
			{
				auto& pos = e.getComponent<CTransform>().pos;
				auto& scale = e.getComponent<CTransform>().scale;
				auto& sprite = e.getComponent<CAnimation>().animation.getSprite();
				sprite.setPosition(pos.x, pos.y);
				sprite.setScale(scale.x, scale.y);
				m_game->window().draw(sprite);
			}
		}
	}
	if (m_drawGrid)
	{
		float leftX = m_game->window().getView().getCenter().x - m_game->getWinWidth() / 2;
		float rightX = leftX + m_game->getWinWidth() + m_gridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

		for (float x = nextGridX; x < rightX; x += m_gridSize.x)
		{
			drawLine(Vec2(x, 0), Vec2(x, m_game->getWinHeight()));
		}

		for (float y = 0; y < m_game->getWinHeight(); y += m_gridSize.y)
		{
			drawLine(Vec2(leftX, m_game->getWinHeight() - y), Vec2(rightX, m_game->getWinHeight() - y));
			for (float x = nextGridX; x < rightX; x += m_gridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
				std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
				m_gridText.setString("(" + xCell + ", " + yCell + ")");
				m_gridText.setPosition(x + 3, m_game->getWinHeight() - y - m_gridSize.y + 2);
				m_game->window().draw(m_gridText);
			}
		}
	}
	if (m_drawCollision)
	{
		for (auto e : m_entityManager.getEntities())
		{
			if (e.hasComponent<CBoundingBox>())
			{
				auto& box = e.getComponent<CBoundingBox>();
				auto& transform = e.getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x, box.size.y));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.pos.x, transform.pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(255, 0, 0, 255));
				rect.setOutlineThickness(1);
				m_game->window().draw(rect);
			}
		}
	}

}

void SceneBase::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		// ui actions
		if (action.name() == "TOGGLE_TEXTURES") { m_drawTextures = !m_drawTextures; }
		else if (action.name() == "TOGGLE_COLLISION_BOXES") { m_drawCollision = !m_drawCollision; }
		else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
		else if (action.name() == "PAUSE") { togglePause(); }
		else if (action.name() == "QUIT") { onEnd(); }
		else if (action.name() == "SHOOT") { spawnBullet(m_player); }

		// player actions
		else if (action.name() == "JUMP") 
		{
			m_player.getComponent<CInput>().up = true;
		}
		else if (action.name() == "LEFT")
		{
			m_player.getComponent<CInput>().left = true;
		}
		else if (action.name() == "RIGHT")
		{
			m_player.getComponent<CInput>().right = true;
		}
		else if (action.name() == "DOWN")
		{
			m_player.getComponent<CInput>().down = true;
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "JUMP")
		{
			m_player.getComponent<CInput>().up = false;
		}
		else if (action.name() == "LEFT")
		{
			m_player.getComponent<CInput>().left = false;
		}
		else if (action.name() == "RIGHT")
		{
			m_player.getComponent<CInput>().right = false;
		}
		else if (action.name() == "DOWN")
		{
			m_player.getComponent<CInput>().down = false;
		}
	}
}

Vec2 SceneBase::gridToMidPixel(float gridX, float gridY, Entity entity)
{
	float x = (gridX * m_gridSize.x) + (entity.getComponent<CAnimation>().animation.getSize().x / 2);
	float y = m_game->getWinHeight() - (gridY * m_gridSize.y) - (entity.getComponent<CAnimation>().animation.getSize().y / 2);
	return Vec2(x, y);
}

void SceneBase::spawnBullet(Entity entity)
{
	// add components
	auto bullet = m_entityManager.addEntity("Bullet");
	bullet.addComponent<CTransform>();
	bullet.addComponent<CGravity>(m_playerConfig.GRAVITY);
	bullet.addComponent<CAnimation>(
		m_game->getAssetManager().getAnimation(m_playerConfig.SHOOT_ANIM), false
	);
	bullet.addComponent<CBoundingBox>(
		bullet.getComponent<CAnimation>().animation.getSize()
	);

	// set components
	bullet.getComponent<CTransform>().pos = entity.getComponent<CTransform>().pos;
	bullet.getComponent<CTransform>().prevPos = bullet.getComponent<CTransform>().pos;
	
	if (entity.getComponent<CTransform>().scale.x < 0)
	{
		bullet.getComponent<CTransform>().velocity = { 10.0f, 10.0f };
	}
	else
	{
		bullet.getComponent<CTransform>().velocity = { -10.0f, 10.0f };
	}
}

void SceneBase::onEnd()
{

}

void SceneBase::drawLine(const Vec2& p1, const Vec2& p2)
{
	sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
	m_game->window().draw(line, 2, sf::Lines);
}
