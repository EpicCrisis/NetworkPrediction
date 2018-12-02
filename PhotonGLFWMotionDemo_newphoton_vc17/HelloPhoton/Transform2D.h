
#pragma once

#include "Vector2.h"
#include "Matrix.h"

struct Transform2D
{
	Vector2 acceleration;
	Vector2 velocity;
	Vector2 position;
	Vector2 scale;
	float rotation;
};


