#pragma once
#include "Entity.h"

namespace Physics
{
	Vec2 GetOverlap(Entity a, Entity b);
	Vec2 GetPreviousOverlap(Entity a, Entity b);
};