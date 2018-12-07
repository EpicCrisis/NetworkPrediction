
#pragma once

#include <GLFW/glfw3.h>
#include "Sprite.h"
#include "GameObject.h"
#include "GameObjectContainer.h"
#include <list>

const int RESOLUTION_X = 800;
const int RESOLUTION_Y = 600;

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

	Vector2 shipSize = Vector2(80.0f, 80.0f);
	Vector2 laserSize = Vector2(20.0f, 60.0f);
	Vector2 rocketSize = Vector2(20.0f, 80.0f);
	Vector2 asteroidSize = Vector2(100.0f, 100.0f);
	Vector2 healthSize = Vector2(50.0f, 50.0f);

	Vector2 shipHalfSize = shipSize * 0.5f;
	Vector2 laserHalfSize = laserSize * 0.5f;
	Vector2 rocketHalfSize = rocketSize * 0.5f;
	Vector2 asteroidHalfSize = asteroidSize * 0.5f;
	Vector2 healthHalfSize = healthSize * 0.5f;

	Sprite m_sprite_ship_red;
	Sprite m_sprite_laser_red;
	Sprite m_sprite_rocket_red;
	Sprite m_sprite_health_red;
	Sprite m_sprite_ship_blue;
	Sprite m_sprite_laser_blue;
	Sprite m_sprite_rocket_blue;
	Sprite m_sprite_health_blue;
	Sprite m_sprite_asteroid;

	GameObject* m_object_ship0;
	GameObject* m_object_ship1;
	GameObject* m_object_asteroid0;
	GameObject* m_object_asteroid1;
	GameObject* m_object_laser0;
	GameObject* m_object_laser1;
	GameObject* m_object_rocket0;
	GameObject* m_object_rocket1;
	GameObject* m_object_health0[3];
	GameObject* m_object_health1[3];

	GameState m_gameState;

	bool doArrangePlayerPositionOnce = false;

	// Ship speed.
	float maxShipSpeed = 200.0f;

	// Ship health.
	int healthShip0 = 3;
	int healthShip1 = 3;

	// Laser fire rate.
	float laserfireRate = 1.0f;
	float laserfireRateCounter = 0.0f;
	bool isLaserUsing = false;
	bool isLaserLoading = false;

	// Rocket fire rate.
	float rocketfireRate = 3.0f;
	float rocketfireRateCounter = 0.0f;
	bool isRocketUsing = false;
	bool isRocketLoading = false;

	// To add delay before ship can be damaged again.
	float immuneDelay = 1.0f;
	float immuneDelayCounter = 0.0f;
	bool isImmune = false;

	// Send this data to show that the remote ship has shot.
	float sendLaserValue = 0.0f;
	float sendRocketValue = 0.0f;
	float returnLaserValue = 0.0f;
	float returnRocketValue = 0.0f;
	float sendHealthValue = 0.0f;
	float returnHealthValue = 0.0f;

	int playerNumber;
	Vector2 mousePosition;

	// Used to calculate local interpolation.
	Vector2 m_lastReceivedPos_ship1;
	Vector2 m_lastReceivedPos_laser1;
	Vector2 m_lastReceivedPos_rocket1;

	float m_lastReceivedRot_ship1;
	float m_lastReceivedRot_laser1;
	float m_lastReceivedRot_rocket1;

	void NetworkUpdate();
	void LimitVelAndPos(GameObject* go);

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

	void InitializeSprites();
	void InitializeObjects();

	void SetPlayerNumber(int playerN);
	void CheckPlayerColour();
	void UpdateObjectCollision();
	float CalculateShipRotation(Vector2 shipPos, Vector2 mousePos);
	void DamageBlink(GameObject* object); // Get the object and make it white, based on immune delay.

	// Separate shoot mechanic into two, one to detect if the projectile is being used,
	// and one to detect if the ship is loading projectile, both must be false to shoot.
	void ShootLaser();
	void ShootRocket();
	void UpdateLocalLaser(float deltaTime);
	void UpdateRemoteLaser(float deltaTime);
	void UpdateLocalRocket(float deltaTime);
	void UpdateRemoteRocket(float deltaTime);
	void UpdateLocalShip(float deltaTime);
	void UpdateRemoteShip(float deltaTime);
	void UpdateRemoteShipHealth();

	bool CheckObjectCollision(GameObject* object0, GameObject* object1);
	bool CheckBorderCollision(GameObject* object, Vector2 minBorder, Vector2 maxBorder);

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

