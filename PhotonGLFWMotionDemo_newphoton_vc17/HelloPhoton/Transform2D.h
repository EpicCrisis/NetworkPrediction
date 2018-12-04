
#pragma once

#include "Vector2.h"
#include "Matrix.h"

struct Transform2D
{
public:
	Vector2 acceleration;
	Vector2 velocity;
	Vector2 position;
	Vector2 scale;
	Vector2 halfSize;
	float rotation;
private:
	Vector2 boundStart;
	Vector2 boundEnd;
};


