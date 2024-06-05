#include "Physics.h"
#include "Components.h"

Vec2 Physics::GetOverlap(Entity a, Entity b)
{
	if (!a.hasComponent<CBoundingBox>() || !b.hasComponent<CBoundingBox>())
	{
		return Vec2(0, 0);
	}

	// create new vector of difference of x's and y's of both entities
	Vec2 delta = { 
		abs(
			a.getComponent<CTransform>().pos.x - b.getComponent<CTransform>().pos.x
			), 
		abs(
			a.getComponent<CTransform>().pos.y - b.getComponent<CTransform>().pos.y
			) 
	};

	// compute overlaps
	float xOverlap = a.getComponent<CBoundingBox>().halfSize.x + b.getComponent<CBoundingBox>().halfSize.x - delta.x;
	float yOverlap = a.getComponent<CBoundingBox>().halfSize.y + b.getComponent<CBoundingBox>().halfSize.y - delta.y;

	return Vec2(xOverlap, yOverlap);
}

Vec2 Physics::GetPreviousOverlap(Entity a, Entity b)
{
	if (!a.hasComponent<CBoundingBox>() || !b.hasComponent<CBoundingBox>())
	{
		return Vec2(0, 0);
	}

	// create new vector of difference of x's and y's of both entities
	Vec2 delta = {
		abs(
			a.getComponent<CTransform>().prevPos.x - b.getComponent<CTransform>().prevPos.x
			),
		abs(
			a.getComponent<CTransform>().prevPos.y - b.getComponent<CTransform>().prevPos.y
			)
	};

	// compute overlaps
	float prevXOverlap = a.getComponent<CBoundingBox>().halfSize.x + b.getComponent<CBoundingBox>().halfSize.x - delta.x;
	float prevYOverlap = a.getComponent<CBoundingBox>().halfSize.y + b.getComponent<CBoundingBox>().halfSize.y - delta.y;

	return Vec2(prevXOverlap, prevYOverlap);
}