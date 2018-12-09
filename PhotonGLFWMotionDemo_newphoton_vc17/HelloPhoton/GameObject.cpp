
#include "GameObject.h"

GameObject::GameObject()
{
	name = "Test";
}

GameObject::GameObject(Sprite& sprite) : GameObject()
{
	SetSprite(sprite);
}

GameObject::~GameObject()
{
}

void GameObject::SetTransform(Transform2D transform)
{
	m_transform = transform;
}

void GameObject::SetTransform(Vector2 position, Vector2 scale, float rotation)
{
	m_transform.position.x = position.x;
	m_transform.position.y = position.y;
	m_transform.scale.x = scale.x;
	m_transform.scale.y = scale.y;
	m_transform.rotation = rotation;
}

void GameObject::SetPosition(Vector2 position)
{
	m_transform.position = position;
}

void GameObject::SetScale(Vector2 scale)
{
	m_transform.scale = scale;
}

void GameObject::SetRotation(float rotation)
{
	m_transform.rotation = rotation;
}

Vector2 GameObject::RotateStartBound(float rotation, Vector2 halfSize, Vector2 startBound)
{
	float rad = (rotation * 3.14159265) / 180.0f;

	float s = sin(rad);
	float c = cos(rad);

	// Translate point back to origin.
	startBound.x -= halfSize.x;
	startBound.y -= halfSize.y;

	// Rotate point.
	Vector2 newPoint;
	newPoint.x = startBound.x * c - startBound.y * s;
	newPoint.y = startBound.x * s + startBound.y * c;

	// Translate points back.
	Vector2 newStartBound;
	newStartBound.x = newPoint.x + halfSize.x;
	newStartBound.y = newPoint.y + halfSize.y;

	return newStartBound;
}

Vector2 GameObject::RotateEndBound(float rotation, Vector2 halfSize, Vector2 endBound)
{
	float rad = (rotation * 3.14159265) / 180.0f;

	float s = sin(rad);
	float c = cos(rad);

	// Translate point back to origin.
	endBound.x -= halfSize.x;
	endBound.y -= halfSize.y;

	// Rotate point.
	Vector2 newPoint1;
	newPoint1.x = endBound.x * c - endBound.y * s;
	newPoint1.y = endBound.x * s + endBound.y * c;

	// Translate points back.
	Vector2 newEndBound;
	newEndBound.x = newPoint1.x + halfSize.x;
	newEndBound.y = newPoint1.y + halfSize.y;

	return newEndBound;
}

void GameObject::SetHalfSize(Vector2 size)
{
	m_transform.halfSize = size;
}

Vector2 GameObject::GetBoundStart()
{
	Vector2 pos = m_transform.position;
	Vector2 halfSize = m_transform.halfSize;

	Vector2 boundStart = pos - halfSize;

	return boundStart;
}

void GameObject::SetBoundStart(Vector2 startBound)
{
}

Vector2 GameObject::GetBoundEnd()
{
	Vector2 pos = m_transform.position;
	Vector2 halfSize = m_transform.halfSize;

	Vector2 boundEnd = pos + halfSize;

	m_transform.boundEnd = boundEnd;

	return boundEnd;
}

void GameObject::SetBoundEnd(Vector2 endBound)
{
}

void GameObject::SetAcceleration(Vector2 acceleration)
{
	m_transform.acceleration = acceleration;
}

void GameObject::SetVelocity(Vector2 velocity)
{
	m_transform.velocity = velocity;
}

void GameObject::SetSprite(Sprite& sprite)
{
	m_sprite = sprite;
}

void GameObject::Start()
{

}

void GameObject::Update(float deltaTime)
{
	m_transform.velocity += m_transform.acceleration * deltaTime;
	m_transform.position += m_transform.velocity * deltaTime;
}

void GameObject::Draw()
{
	GetSprite().Draw(GetTransform());
}
