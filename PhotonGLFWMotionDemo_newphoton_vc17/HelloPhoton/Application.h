
#pragma once

#include "Sprite.h"
#include "GameObject.h"
#include "GameObjectContainer.h"
#include <list>

enum GameState
{
	STATE_WAITGAME = 0,
	STATE_STARTGAME = 1,
	STATE_GAMEOVER
};

class Application
{
protected:
	GameObjectContainer<GameObject> m_GOs;
	std::list<GameObject*>::iterator iteGO;

	Sprite m_sprite_ship_red;
	Sprite m_sprite_laser_red;
	Sprite m_sprite_rocket_red;
	Sprite m_sprite_ship_blue;
	Sprite m_sprite_laser_blue;
	Sprite m_sprite_rocket_blue;

	GameObject* m_object_ship0;
	GameObject* m_object_ship1;

	GameState m_gameState;

	int playerNumber;
	Vector2 mousePosition;
	Vector2 m_lastReceivedPos;
	double m_prevReceivedTime;

	void networkUpdate();
	void limitVelAndPos(GameObject* go);

public:
	Application();
	~Application();

	// Create singleton.
	static Application& getInstance()
	{
		static Application application;
		return application;
	}

	// Overload spawn gameobject function.
	GameObject* Spawn();
	GameObject* Spawn(GameObject* duplicate);
	GameObject* Spawn(Transform2D transform);
	GameObject* Spawn(Vector2 position, float rotation, Vector2 scale);
	GameObject& FindGameObject(int index);
	void Destroy(GameObject* gameObject);

	void SetGameState(GameState state);
	GameState GetGameState();

	void SetPlayerNumber(int playerN);
	void CheckPlayerColour();

	void SendMyData(void);
	void OnReceivedOpponentData(float* data);

	virtual void OnKeyPressed(int key);
	virtual void OnKeyReleased(int key);
	virtual void OnKeyHold(int key);
	virtual void OnMousePressed(int button);
	virtual void OnMouseReleased(int button);
	virtual void OnMouseHold(int button);
	virtual void OnMouseMoved(double mousePosX, double mousePosY);

	virtual void Start();
	virtual void Update(double elapsedTime);
	virtual void Draw();
};

