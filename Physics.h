#pragma once
#include "Entity.h"

namespace Physics
{
	Vec2 GetOverlap(const std::shared_ptr<Entity> a, const std::shared_ptr<Entity> b);
	Vec2 GetPreviousOverlap(const std::shared_ptr<Entity> a, const std::shared_ptr<Entity> b);
};