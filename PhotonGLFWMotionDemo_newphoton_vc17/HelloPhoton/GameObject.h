
#pragma once

#include "Sprite.h"
#include "Transform2D.h"
#include <string>

class GameObject
{
protected:
	Transform2D m_transform;
	Sprite m_sprite;

public:
	std::string name;

	GameObject();
	GameObject(Sprite& sprite);
	~GameObject();

	Transform2D GetTransform() { return m_transform; };
	void SetTransform(Transform2D transform);
	void SetTransform(Vector2 position, Vector2 scale, float rotation);

	void SetPosition(Vector2 position);
	void SetScale(Vector2 scale);
	void SetRotation(float rotation);

	// Must divide by half because position starts in the center.
	Vector2 GetHalfSize() { return m_transform.halfSize; };
	void SetHalfSize(Vector2 size);

	// Get the bottom left corner and top right corner.
	Vector2 GetBoundStart();
	Vector2 GetBoundEnd();

	Vector2 GetAcceleration() { return m_transform.acceleration; };
	void SetAcceleration(Vector2 acceleration);

	Vector2 GetVelocity() { return m_transform.velocity; };
	void SetVelocity(Vector2 velocity);

	Sprite& GetSprite() { return m_sprite; };
	void SetSprite(Sprite& sprite);

	void Start();
	void Update(float deltaTime);
	void Draw();
};

