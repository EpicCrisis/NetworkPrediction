
#include "Vector2.h"

Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const Vector2 & v) : x(v.x), y(v.y)
{
}

Vector2::Vector2(const Vector2 * v) : x(v->x), y(v->y)
{
}

Vector2::~Vector2()
{
}
