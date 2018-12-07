
#include "Application.h"
#include "MyPhoton.h"

// The higher networkFPS, the higher bandwidth requirement of your game.
// ** about 30FPS is common data sync rate for real time games
// ** slower-paced action games could use as low as 10 FPS

const float networkFPS = 30.0f;
const float gNetworkFrameTime = 1.0f / networkFPS;

Application::Application()
{
}

Application::~Application()
{
}

GameObject* Application::Spawn()
{
	return Spawn(new GameObject());
}

GameObject* Application::Spawn(GameObject * duplicate)
{
	m_GOs.GetList().push_back(duplicate);
	return duplicate;
}

GameObject* Application::Spawn(Transform2D transform)
{
	GameObject* GO = new GameObject();
	GO->SetTransform(transform);

	return Spawn(GO);
}

GameObject* Application::Spawn(Vector2 position, float rotation, Vector2 scale)
{
	Transform2D transform;
	transform.position = position;
	transform.rotation = rotation;
	transform.scale = scale;

	return Spawn(transform);
}

GameObject& Application::FindGameObject(int index)
{
	return m_GOs.GetItem(index);
}

void Application::Destroy(GameObject * gameObject)
{
	GameObject* storedGOs = gameObject;
	m_GOs.GetList().remove(gameObject);
	delete gameObject;
}

void Application::SetGameState(GameState gameState)
{
	m_gameState = gameState;
}

GameState Application::GetGameState()
{
	return m_gameState;
}

void Application::NetworkUpdate()
{
	static double prevTime = glfwGetTime();

	double time = glfwGetTime();
	if (time - prevTime >= gNetworkFrameTime)
	{
		SendMyData();
		prevTime = time;
	}
}

void Application::LimitVelAndPos(GameObject* go)
{
	if (go->GetVelocity().Length() > maxShipSpeed)
	{
		Vector2 vec = go->GetVelocity();
		vec.Normalize();
		vec *= maxShipSpeed;
		go->SetVelocity(vec);
	}
}

void Application::InitializeSprites()
{
	m_sprite_ship_red.SetFilePath("../media/Spaceship_Red.bmp");
	m_sprite_ship_blue.SetFilePath("../media/Spaceship_Blue.bmp");
	m_sprite_laser_red.SetFilePath("../media/Laser_Red.bmp");
	m_sprite_laser_blue.SetFilePath("../media/Laser_Blue.bmp");
	m_sprite_rocket_red.SetFilePath("../media/Rocket_Red.bmp");
	m_sprite_rocket_blue.SetFilePath("../media/Rocket_Blue.bmp");
	m_sprite_asteroid.SetFilePath("../media/Asteroid.bmp");
	m_sprite_health_red.SetFilePath("../media/Health_Red.bmp");
	m_sprite_health_blue.SetFilePath("../media/Health_Blue.bmp");

	m_sprite_ship_red.SetDimension(shipSize.x, shipSize.y);
	m_sprite_ship_blue.SetDimension(shipSize.x, shipSize.y);
	m_sprite_laser_red.SetDimension(laserSize.x, laserSize.y);
	m_sprite_laser_blue.SetDimension(laserSize.x, laserSize.y);
	m_sprite_rocket_red.SetDimension(rocketSize.x, rocketSize.y);
	m_sprite_rocket_blue.SetDimension(rocketSize.x, rocketSize.y);
	m_sprite_asteroid.SetDimension(asteroidSize.x, asteroidSize.y);
	m_sprite_health_blue.SetDimension(healthSize.x, healthSize.y);
	m_sprite_health_red.SetDimension(healthSize.x, healthSize.y);
}

void Application::InitializeObjects()
{
	// Spawn and set sprite for the objects.
	m_object_ship0 = Spawn(Vector2(100.0f, 300.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_ship1 = Spawn(Vector2(700.0f, 300.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_ship0->SetSprite(m_sprite_ship_red);
	m_object_ship1->SetSprite(m_sprite_ship_blue);
	
	m_object_laser0 = Spawn(Vector2(-100.0f, -100.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_laser1 = Spawn(Vector2(100.0f, 100.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_laser0->SetSprite(m_sprite_laser_red);
	m_object_laser1->SetSprite(m_sprite_laser_blue);

	m_object_rocket0 = Spawn(Vector2(-100.0f, -100.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_rocket1 = Spawn(Vector2(-100.0f, -100.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_rocket0->SetSprite(m_sprite_rocket_red);
	m_object_rocket1->SetSprite(m_sprite_rocket_blue);

	m_object_asteroid0 = Spawn(Vector2(200.0f, 400.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_asteroid1 = Spawn(Vector2(600.0f, 200.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_asteroid0->SetSprite(m_sprite_asteroid);
	m_object_asteroid1->SetSprite(m_sprite_asteroid);

	for (int i = 0; i < healthShip0; ++i)
	{
		m_object_health0[i] = Spawn(Vector2(60.0f + (i * 60.0f), 540.0f), 0.0f, Vector2(1.0f, 1.0f));
		m_object_health0[i]->SetSprite(m_sprite_health_red);
		m_object_health0[i]->SetHalfSize(healthHalfSize);
	}
	for (int i = 0; i < healthShip1; ++i)
	{
		m_object_health1[i] = Spawn(Vector2(740.0f + (i * -60.0f), 540.0f), 0.0f, Vector2(1.0f, 1.0f));
		m_object_health1[i]->SetSprite(m_sprite_health_blue);
		m_object_health1[i]->SetHalfSize(healthHalfSize);
	}

	// Set the size for the objects.
	m_object_ship0->SetHalfSize(shipHalfSize);
	m_object_ship1->SetHalfSize(shipHalfSize);

	m_object_laser0->SetHalfSize(laserHalfSize);
	m_object_laser1->SetHalfSize(laserHalfSize);

	m_object_rocket0->SetHalfSize(rocketHalfSize);
	m_object_rocket1->SetHalfSize(rocketHalfSize);

	m_object_asteroid0->SetHalfSize(asteroidHalfSize);
	m_object_asteroid1->SetHalfSize(asteroidHalfSize);
}

void Application::SetPlayerNumber(int playerN)
{
	if (playerNumber == 0)
	{
		playerNumber = playerN;
		std::cout << playerNumber << std::endl;
	}
}

// Change the colors of ship, laser, and rocket based on player.
void Application::CheckPlayerColour()
{
	if (playerNumber == 1)
	{
		m_object_ship0->SetSprite(m_sprite_ship_red);
		m_object_ship1->SetSprite(m_sprite_ship_blue);

		m_object_laser0->SetSprite(m_sprite_laser_red);
		m_object_laser1->SetSprite(m_sprite_laser_blue);

		m_object_rocket0->SetSprite(m_sprite_rocket_red);
		m_object_rocket1->SetSprite(m_sprite_rocket_blue);

		for (int i = 0; i < healthShip0; ++i)
		{
			m_object_health0[i]->SetPosition(Vector2(60.0f + (i * 60.0f), 540.0f));
			m_object_health1[i]->SetPosition(Vector2(740.0f + (i * -60.0f), 540.0f));

			m_object_health0[i]->SetSprite(m_sprite_health_red);
			m_object_health1[i]->SetSprite(m_sprite_health_blue);
		}
	}
	else if (playerNumber == 2)
	{
		m_object_ship0->SetSprite(m_sprite_ship_blue);
		m_object_ship1->SetSprite(m_sprite_ship_red);

		m_object_laser0->SetSprite(m_sprite_laser_blue);
		m_object_laser1->SetSprite(m_sprite_laser_red);

		m_object_rocket0->SetSprite(m_sprite_rocket_blue);
		m_object_rocket1->SetSprite(m_sprite_rocket_red);

		for (int i = 0; i < healthShip1; ++i)
		{
			m_object_health0[i]->SetPosition(Vector2(740.0f + (i * -60.0f), 540.0f));
			m_object_health1[i]->SetPosition(Vector2(60.0f + (i * 60.0f), 540.0f));

			m_object_health0[i]->SetSprite(m_sprite_health_blue);
			m_object_health1[i]->SetSprite(m_sprite_health_red);
		}
	}

	m_object_ship0->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
	m_object_ship1->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
	m_object_laser0->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
	m_object_laser1->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
	m_object_rocket0->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
	m_object_rocket1->GetSprite().SetBlendingMode(BLEND_ADDITIVE);

	for (int i = 0; i < 3; ++i)
	{
		m_object_health0[i]->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
		m_object_health1[i]->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
	}
}

void Application::UpdateObjectCollision()
{
	if (CheckBorderCollision(m_object_ship0, Vector2(0.0f, 0.0f), Vector2(RESOLUTION_X, RESOLUTION_Y)))
	{
		m_object_ship0->SetVelocity(m_object_ship0->GetVelocity() * -1.0f);
	}
	if (CheckObjectCollision(m_object_ship0, m_object_asteroid0))
	{
		m_object_ship0->SetVelocity(m_object_ship0->GetVelocity() * -1.0f);
	}
	if (CheckObjectCollision(m_object_ship0, m_object_asteroid1))
	{
		m_object_ship0->SetVelocity(m_object_ship0->GetVelocity() * -1.0f);
	}
	if (CheckObjectCollision(m_object_ship0, m_object_ship1))
	{
		m_object_ship0->SetVelocity(m_object_ship0->GetVelocity() * -1.0f);
	}
	if (CheckObjectCollision(m_object_ship1, m_object_laser0))
	{
		m_object_laser0->SetTransform(Vector2(-100.0f, -100.0f), Vector2(1.0f, 1.0f), 0.0f);
		m_object_laser0->SetAcceleration(Vector2(0.0f, 0.0f));
		m_object_laser0->SetVelocity(Vector2(0.0f, 0.0f));

		if (healthShip1 >= 0)
		{
			healthShip1 -= 1;
			Destroy(m_object_health1[healthShip1]);
		}
		isLaserUsing = false;
	}
	if (CheckObjectCollision(m_object_ship1, m_object_rocket0))
	{
		m_object_rocket0->SetTransform(Vector2(-100.0f, -100.0f), Vector2(1.0f, 1.0f), 0.0f);
		m_object_rocket0->SetAcceleration(Vector2(0.0f, 0.0f));
		m_object_rocket0->SetVelocity(Vector2(0.0f, 0.0f));

		if (healthShip1 >= 0)
		{
			healthShip1 -= 1;
			Destroy(m_object_health1[healthShip1]);
		}
		isRocketUsing = false;
	}
}

float Application::CalculateShipRotation(Vector2 shipPos, Vector2 mousePos)
{
	Vector2 dir = Vector2(mousePos - shipPos);
	dir.Normalize();
	float angle = (atan2(dir.x, dir.y) * 180.0f) / 3.142f;
	return angle;
}

void Application::DamageBlink(GameObject * object)
{
}

void Application::ShootLaser()
{
	if (!isLaserUsing && !isLaserLoading)
	{
		m_object_laser0->SetPosition
		(
			Vector2
			(
				m_object_ship0->GetPosition().x,
				m_object_ship0->GetPosition().y
			)
		);

		m_object_laser0->SetRotation(m_object_ship0->GetRotation());

		isLaserUsing = true;
		isLaserLoading = true;

		sendLaserValue = 2.0f;
	}
}

void Application::ShootRocket()
{
	if (!isRocketUsing && !isRocketLoading)
	{
		m_object_rocket0->SetPosition
		(
			Vector2
			(
				m_object_ship0->GetPosition().x,
				m_object_ship0->GetPosition().y
			)
		);

		m_object_rocket0->SetRotation(m_object_ship0->GetRotation());

		isRocketUsing = true;
		isRocketLoading = true;

		sendRocketValue = 2.0f;
	}
}

void Application::UpdateLaser(float deltaTime)
{
	// Local laser fire rate.
	if (isLaserLoading)
	{
		laserfireRateCounter += deltaTime;
		if (laserfireRateCounter > laserfireRate)
		{
			laserfireRateCounter = 0.0f;
			isLaserLoading = false;
		}
	}

	// Remote laser.
	m_object_laser1->Update(deltaTime);

	if (returnLaserValue >= 1.0f)
	{
		m_object_laser1->SetPosition
		(
			m_lastReceivedPos_laser1
		);
		returnLaserValue = 0.0f;
	}
	else
	{
		m_object_laser1->SetPosition
		(
			m_object_laser1->GetPosition() * 0.995f + m_lastReceivedPos_laser1 * 0.005f
		);
	}
	m_object_laser1->SetRotation(m_lastReceivedRot_laser1);
}

void Application::UpdateRocket(float deltaTime)
{
	// Local rocket fire rate.
	if (isRocketLoading)
	{
		rocketfireRateCounter += deltaTime;
		if (rocketfireRateCounter > rocketfireRate)
		{
			rocketfireRateCounter = 0.0f;
			isRocketLoading = false;
		}
	}

	// Remote rocket.
	m_object_rocket1->Update(deltaTime);

	if (returnRocketValue >= 1.0f)
	{
		m_object_rocket1->SetPosition
		(
			m_lastReceivedPos_rocket1
		);
		returnRocketValue = 0.0f;
	}
	m_object_rocket1->SetPosition
	(
		m_object_rocket1->GetPosition() * 0.995f + m_lastReceivedPos_rocket1 * 0.005f
	);
	m_object_rocket1->SetRotation(m_lastReceivedRot_rocket1);
}

void Application::UpdateLocalShip(float deltaTime)
{
	m_object_ship0->SetRotation
	(
		CalculateShipRotation
		(
			m_object_ship0->GetPosition(), mousePosition
		)
	);

	m_object_ship0->Update(deltaTime);
	m_object_ship0->SetAcceleration(Vector2(0.0f, 0.0f));
	LimitVelAndPos(m_object_ship0);
}

void Application::UpdateRemoteShip(float deltaTime)
{
	m_object_ship1->SetRotation
	(
		m_lastReceivedRot_ship1
	);

	// update remote ship.
	m_object_ship1->Update(deltaTime);
	// very slowly interpolate from on-going predicting pos to lastest received pos. 
	// Without this interpolation, the offset of opponent position will keep being accumulated. 
	m_object_ship1->SetPosition
	(
		m_object_ship1->GetPosition() * 0.995f + m_lastReceivedPos_ship1 * 0.005f
	);
	LimitVelAndPos(m_object_ship1);
}

bool Application::CheckObjectCollision(GameObject* object0, GameObject* object1)
{
	// Min max 0
	Vector2 min_0 = object0->GetBoundStart();
	Vector2 max_0 = object0->GetBoundEnd();

	// Min max 1
	Vector2 min_1 = object1->GetBoundStart();
	Vector2 max_1 = object1->GetBoundEnd();

	// Collision tests
	if (max_0.x < min_1.x || min_0.x > max_1.x)
	{
		return false;
	}
	if (max_0.y < min_1.y || min_0.y > max_1.y)
	{
		return false;
	}

	return true;
}

bool Application::CheckBorderCollision(GameObject* object, Vector2 minBorder, Vector2 maxBorder)
{
	// Min max 0
	Vector2 min_0 = object->GetBoundStart();
	Vector2 max_0 = object->GetBoundEnd();

	// Min max 1
	Vector2 min_border = minBorder;
	Vector2 max_border = maxBorder;

	// Collision tests
	if (max_0.x > maxBorder.x || min_0.x < minBorder.x)
	{
		return true;
	}
	if (max_0.y > maxBorder.y || min_0.y < minBorder.y)
	{
		return true;
	}

	return false;
}

void Application::SendMyData(void)
{
	Vector2 pos_ship0 = m_object_ship0->GetPosition();
	Vector2 vel_ship0 = m_object_ship0->GetVelocity();
	Vector2 acc_ship0 = m_object_ship0->GetAcceleration();
	Vector2 pos_laser0 = m_object_laser0->GetPosition();
	Vector2 vel_laser0 = m_object_laser0->GetVelocity();
	Vector2 acc_laser0 = m_object_laser0->GetAcceleration();
	Vector2 pos_rocket0 = m_object_rocket0->GetPosition();
	Vector2 vel_rocket0 = m_object_rocket0->GetVelocity();
	Vector2 acc_rocket0 = m_object_rocket0->GetAcceleration();
	float shotLaser = sendLaserValue;
	float shotRocket = sendRocketValue;
	float shipRot = m_object_ship0->GetRotation();
	float laserRot = m_object_laser0->GetRotation();
	float rocketRot = m_object_rocket0->GetRotation();

	MyPhoton::getInstance().sendMyData
	(
		pos_ship0, vel_ship0, acc_ship0,
		pos_laser0, vel_laser0, acc_laser0,
		pos_rocket0, vel_rocket0, acc_rocket0,
		shotLaser, shotRocket, 
		shipRot, laserRot, rocketRot
	);
}

// Things to update...
// Ship position, ship rotation, laser position, laser rotation, ship health
void Application::OnReceivedOpponentData(float* data)
{
	if (m_gameState == GameState::STATE_WAITGAME)
	{
		if (!doArrangePlayerPositionOnce)
		{
			doArrangePlayerPositionOnce = true;

			if (playerNumber == 1)
			{
				m_object_ship0->SetPosition(Vector2(100.0f, 300.0f));
			}
			else if (playerNumber == 2)
			{
				m_object_ship0->SetPosition(Vector2(700.0f, 300.0f));
			}

			CheckPlayerColour();
		}

		m_gameState = GameState::STATE_STARTGAME;

		// ship : 0 ~ 5, laser0 : 6 ~ 11, rocket0 : 12 ~ 17, shotLaser : 18, shotRocket : 19
		// shipRot : 20, laserRot : 21, rocketRot : 22

		return;
	}

	// remote ship data.
	m_lastReceivedPos_ship1 = Vector2(data[0], data[1]);
	m_object_ship1->SetVelocity(Vector2(data[2], data[3]));
	m_object_ship1->SetAcceleration(Vector2(data[4], data[5]));

	m_lastReceivedPos_laser1 = Vector2(data[6], data[7]);
	m_object_laser1->SetVelocity(Vector2(data[8], data[9]));
	m_object_laser1->SetAcceleration(Vector2(data[10], data[11]));

	m_lastReceivedPos_rocket1 = Vector2(data[12], data[13]);
	m_object_rocket1->SetVelocity(Vector2(data[14], data[15]));
	m_object_rocket1->SetAcceleration(Vector2(data[16], data[17]));

	returnLaserValue = data[18];
	returnRocketValue = data[19];
	
	m_lastReceivedRot_ship1 = data[20];
	m_lastReceivedRot_laser1 = data[21];
	m_lastReceivedRot_rocket1 = data[22];
}

void Application::OnKeyPressed(int key)
{
	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}
}

void Application::OnKeyReleased(int key)
{
	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}
}

void Application::OnKeyHold(int key)
{
	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}

	if (key == GLFW_KEY_W)
	{
		m_object_ship0->SetAcceleration
		(
			m_object_ship0->GetAcceleration() + Vector2(0.0f, 400.0f)
		);
	}
	if (key == GLFW_KEY_A)
	{
		m_object_ship0->SetAcceleration
		(
			m_object_ship0->GetAcceleration() + Vector2(-400.0f, 0.0f)
		);
	}
	if (key == GLFW_KEY_S)
	{
		m_object_ship0->SetAcceleration
		(
			m_object_ship0->GetAcceleration() + Vector2(0.0f, -400.0f)
		);
	}
	if (key == GLFW_KEY_D)
	{
		m_object_ship0->SetAcceleration
		(
			m_object_ship0->GetAcceleration() + Vector2(400.0f, 0.0f)
		);
	}
}

void Application::OnMousePressed(int button)
{
	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}

	// Shoot laser
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		ShootLaser();
	}
	// Shoot rocket
	if (button == GLFW_MOUSE_BUTTON_2)
	{
		ShootRocket();
	}
}

void Application::OnMouseReleased(int button)
{
	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}
}

void Application::OnMouseHold(int button)
{
	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}

	if (button == GLFW_MOUSE_BUTTON_1)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_2)
	{

	}
}

void Application::OnMouseMoved(double mousePosX, double mousePosY)
{
	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}

	// Here should have an update that allows the head of the ship to look at the mouse position.
	// Calculate rotation based on mouse position.
	mousePosition = Vector2(mousePosX, ((-mousePosY) + RESOLUTION_Y));
}

void Application::Start()
{
	srand(time(0));

	m_gameState = GameState::STATE_WAITGAME;

	MyPhoton::getInstance().connect();

	InitializeSprites();
	InitializeObjects();
}

void Application::Update(double elapsedTime)
{
	MyPhoton::getInstance().run();

	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}

	UpdateObjectCollision();

	UpdateLocalShip(elapsedTime);

	NetworkUpdate();

	UpdateRemoteShip(elapsedTime);

	UpdateLaser(elapsedTime);
	UpdateRocket(elapsedTime);
}

void Application::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}

	// Draw function
	iteGO = m_GOs.GetList().begin();
	while (iteGO != m_GOs.GetList().end())
	{
		GameObject GO = **iteGO;
		GO.Draw();
		++iteGO;
	}
}
