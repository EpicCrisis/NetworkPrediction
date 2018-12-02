
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
