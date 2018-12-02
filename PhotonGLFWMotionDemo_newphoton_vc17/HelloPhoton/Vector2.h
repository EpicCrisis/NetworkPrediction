#pragma once

#include <math.h>

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2 & v);
	Vector2(const Vector2 * v);
	~Vector2();

	// Assigning equality operators
	inline Vector2 operator = (const Vector2 & v) { x = v.x; y = v.y; return *this; }
	inline Vector2 operator = (const float & f) { x = f; y = f; return *this; }
	inline Vector2 operator - (void) { x = -x; y = -y; return *this; }
	inline bool operator == (const Vector2 & v) { return (x == v.x) && (y == v.y); }
	inline bool operator != (const Vector2 & v) { return (x != v.x) && (y != v.y); }
	inline bool operator > (const Vector2 & v) { return (x > v.x) && (y > v.y); }
	inline bool operator < (const Vector2 & v) { return (x < v.x) && (y < v.y); }
	inline bool operator >= (const Vector2 & v) { return (x >= v.x) && (y >= v.y); }
	inline bool operator <= (const Vector2 & v) { return (x <= v.x) && (y <= v.y); }

	// Vector2 to vector2 operators
	inline const Vector2 operator + (const Vector2 & v) const { return Vector2(x + v.x, y + v.y); }
	inline const Vector2 operator - (const Vector2 & v) const { return Vector2(x - v.x, y - v.y); }
	inline const Vector2 operator * (const Vector2 & v) const { return Vector2(x * v.x, y * v.y); }
	inline const Vector2 operator / (const Vector2 & v) const { return Vector2(x / v.x, y / v.y); }

	// Vector2 to this operators
	inline Vector2 & operator += (const Vector2 & v) { x += v.x; y += v.y; return *this; }
	inline Vector2 & operator -= (const Vector2 & v) { x -= v.x; y -= v.y; return *this; }
	inline Vector2 & operator *= (const Vector2 & v) { x *= v.x; y *= v.y; return *this; }
	inline Vector2 & operator /= (const Vector2 & v) { x /= v.x; y /= v.y; return *this; }

	// Scalar to vector2 operators
	inline const Vector2 operator + (float f) const { return Vector2(x + f, y + f); }
	inline const Vector2 operator - (float f) const { return Vector2(x - f, y - f); }
	inline const Vector2 operator * (float f) const { return Vector2(x * f, y * f); }
	inline const Vector2 operator / (float f) const { return Vector2(x / f, y / f); }

	// Scalar to this operators
	inline Vector2 & operator += (float f) { x += f; y += f; return *this; }
	inline Vector2 & operator -= (float f) { x -= f; y -= f; return *this; }
	inline Vector2 & operator *= (float f) { x *= f; y *= f; return *this; }
	inline Vector2 & operator /= (float f) { x /= f; y /= f; return *this; }

	// Get squared length
	inline float SquaredLength() const { return x * x + y * y; }

	// Get length
	inline float Length() const { return sqrt(SquaredLength()); }

	// Normalize.
	inline void Normalize()
	{
		float l = Length();
		if (l > 0)
		{
			float invL = 1.0f / l;
			x *= invL;
			y *= invL;
		}
	}
};