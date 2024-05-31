#pragma once
#include "Vec2.h"
#include "Animation.h"

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2	prevPos = { 0.0f, 0.0f };	// pos in the previous frame
	Vec2	pos = { 0.0f, 0.0f };
	Vec2	velocity = { 0.0f, 0.0f };
	Vec2	scale = { 1.0f, 1.0f };
	float	angle = 0.0f;

	CTransform()
	{}
	CTransform(const Vec2& p)
		: pos(p)
		, prevPos(p)
	{}
	CTransform(const Vec2& p, const Vec2& v, const Vec2& s, float a)
		: prevPos(p)
		, pos(p)
		, velocity(v)
		, scale(s)
		, angle(a)
	{}

};

class CInput : public Component
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	bool canShoot = true;
	bool canJump = true;

	CInput()
	{}
};

class CLifespan : public Component
{
public:
	int remaining = 0;
	int total = 0;

	CLifespan()
	{}

	CLifespan(int total)
		: remaining(total)
		, total(total)
	{}
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;

	CAnimation()
	{}
	CAnimation(const Animation& animation, bool repeat)
		: animation(animation)
		, repeat(repeat)
	{}
};

class CGravity : public Component
{
public:
	float gravity = 0;

	CGravity()
	{}
	CGravity(float g)
		: gravity(g)
	{}
};

class CState : public Component
{
public:
	std::string state = "None";

	CState()
	{}

	CState(const std::string& state)
		: state(state)
	{}
};

class CBoundingBox : public Component
{
public:
	Vec2 size;
	Vec2 halfSize;

	CBoundingBox()
	{}

	CBoundingBox(const Vec2& size)
		: size(size)
		, halfSize(size.x / 2.0f, size.y / 2.0f)
	{}
};