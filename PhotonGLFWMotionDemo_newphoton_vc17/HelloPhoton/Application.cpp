
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

void Application::LimitVelAndPos(GameObject* go, float limit)
{
	if (go->GetVelocity().Length() > maxShipSpeed)
	{
		Vector2 vec = go->GetVelocity();
		vec.Normalize();
		vec *= limit;
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
	m_sprite_win.SetFilePath("../media/You_Win.bmp");
	m_sprite_lose.SetFilePath("../media/You_Lose.bmp");

	m_sprite_ship_red.SetDimension(shipSize.x, shipSize.y);
	m_sprite_ship_blue.SetDimension(shipSize.x, shipSize.y);
	m_sprite_laser_red.SetDimension(laserSize.x, laserSize.y);
	m_sprite_laser_blue.SetDimension(laserSize.x, laserSize.y);
	m_sprite_rocket_red.SetDimension(rocketSize.x, rocketSize.y);
	m_sprite_rocket_blue.SetDimension(rocketSize.x, rocketSize.y);
	m_sprite_asteroid.SetDimension(asteroidSize.x, asteroidSize.y);
	m_sprite_health_blue.SetDimension(healthSize.x, healthSize.y);
	m_sprite_health_red.SetDimension(healthSize.x, healthSize.y);
	m_sprite_win.SetDimension(winSize.x, winSize.y);
	m_sprite_lose.SetDimension(loseSize.x, loseSize.y);
}

void Application::InitializeObjects()
{
	// Spawn and set sprite for the objects.
	m_object_ship0 = Spawn(Vector2(100.0f, 300.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_ship1 = Spawn(Vector2(700.0f, 300.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_ship0->SetSprite(m_sprite_ship_red);
	m_object_ship1->SetSprite(m_sprite_ship_blue);
	
	m_object_laser0 = Spawn(Vector2(-200.0f, -200.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_laser1 = Spawn(Vector2(-200.0f, -200.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_laser0->SetSprite(m_sprite_laser_red);
	m_object_laser1->SetSprite(m_sprite_laser_blue);

	m_object_rocket0 = Spawn(Vector2(-200.0f, -200.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_rocket1 = Spawn(Vector2(-200.0f, -200.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_rocket0->SetSprite(m_sprite_rocket_red);
	m_object_rocket1->SetSprite(m_sprite_rocket_blue);

	m_object_asteroid0 = Spawn(Vector2(200.0f, 400.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_asteroid1 = Spawn(Vector2(600.0f, 200.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_asteroid0->SetSprite(m_sprite_asteroid);
	m_object_asteroid1->SetSprite(m_sprite_asteroid);

	m_object_winLose = Spawn(Vector2(-200.0f, -200.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_winLose->SetSprite(m_sprite_win);

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
	if (playerNumber == 2)
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
	// Laser collision here. //
	if (CheckBorderCollision(m_object_laser0, Vector2(0.0f, 0.0f), Vector2(RESOLUTION_X, RESOLUTION_Y)))
	{
		m_object_laser0->SetPosition(Vector2(-200.0f, -200.0f));
		m_object_laser0->SetAcceleration(Vector2(0.0f, 0.0f));
		m_object_laser0->SetVelocity(Vector2(0.0f, 0.0f));

		isLaserUsing = false;
	}
	if (CheckObjectCollision(m_object_laser0, m_object_asteroid0))
	{
		m_object_laser0->SetPosition(Vector2(-200.0f, -200.0f));
		m_object_laser0->SetAcceleration(Vector2(0.0f, 0.0f));
		m_object_laser0->SetVelocity(Vector2(0.0f, 0.0f));

		isLaserUsing = false;
	}
	if (CheckObjectCollision(m_object_laser0, m_object_asteroid1))
	{
		m_object_laser0->SetPosition(Vector2(-200.0f, -200.0f));
		m_object_laser0->SetAcceleration(Vector2(0.0f, 0.0f));
		m_object_laser0->SetVelocity(Vector2(0.0f, 0.0f));

		isLaserUsing = false;
	}
	// . //

	// Rocket collision here. //
	if (CheckBorderCollision(m_object_rocket0, Vector2(0.0f, 0.0f), Vector2(RESOLUTION_X, RESOLUTION_Y)))
	{
		m_object_rocket0->SetPosition(Vector2(-200.0f, -200.0f));
		m_object_rocket0->SetAcceleration(Vector2(0.0f, 0.0f));
		m_object_rocket0->SetVelocity(Vector2(0.0f, 0.0f));

		isRocketUsing = false;
	}
	if (CheckObjectCollision(m_object_rocket0, m_object_asteroid0))
	{
		m_object_rocket0->SetPosition(Vector2(-200.0f, -200.0f));
		m_object_rocket0->SetAcceleration(Vector2(0.0f, 0.0f));
		m_object_rocket0->SetVelocity(Vector2(0.0f, 0.0f));

		isRocketUsing = false;
	}
	if (CheckObjectCollision(m_object_rocket0, m_object_asteroid1))
	{
		m_object_rocket0->SetPosition(Vector2(-200.0f, -200.0f));
		m_object_rocket0->SetAcceleration(Vector2(0.0f, 0.0f));
		m_object_rocket0->SetVelocity(Vector2(0.0f, 0.0f));

		isRocketUsing = false;
	}
	// . //

	// Remote ship collision with local projectile, update local projectile. //
	//if (CheckObjectCollision(m_object_ship1, m_object_laser0))
	//{
	//	m_object_laser0->SetPosition(Vector2(-200.0f, -200.0f));
	//	m_object_laser0->SetVelocity(Vector2(0.0f, 0.0f));
	//	m_object_laser0->SetAcceleration(Vector2(0.0f, 0.0f));
	//}
	//if (CheckObjectCollision(m_object_ship1, m_object_rocket0))
	//{
	//	m_object_rocket0->SetPosition(Vector2(-200.0f, -200.0f));
	//	m_object_rocket0->SetVelocity(Vector2(0.0f, 0.0f));
	//	m_object_rocket0->SetAcceleration(Vector2(0.0f, 0.0f));
	//}

	// Local ship collision with remote projectile, activate immunity. //
	if (!isImmune)
	{
		if (CheckObjectCollision(m_object_ship0, m_object_laser1))
		{
			isImmune = true;

			if (healthShip0 > 0)
			{
				healthShip0 -= 1;
			}
			m_object_laser1->SetPosition(Vector2(-200.0f, -200.0f));
			m_object_laser1->SetVelocity(Vector2(0.0f, 0.0f));
			m_object_laser1->SetAcceleration(Vector2(0.0f, 0.0f));
			
			DamageBlink(m_object_ship0, hurtColor);
		}
		if (CheckObjectCollision(m_object_ship0, m_object_rocket1))
		{
			isImmune = true;

			if (healthShip0 > 0)
			{
				healthShip0 -= 1;
			}
			m_object_rocket1->SetPosition(Vector2(-200.0f, -200.0f));
			m_object_rocket1->SetVelocity(Vector2(0.0f, 0.0f));
			m_object_rocket1->SetAcceleration(Vector2(0.0f, 0.0f));

			DamageBlink(m_object_ship0, hurtColor);
		}
	}
	// . //

	// Local ship collision here. //
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
	// . //
}

float Application::CalculatePointRotation(Vector2 object, Vector2 mousePos)
{
	Vector2 dir = Vector2(mousePos - object);
	dir.Normalize();
	float angle = (atan2(dir.x, dir.y) * 180.0f) / 3.142f;
	return angle;
}

void Application::DamageBlink(GameObject* object, Color color)
{
	object->GetSprite().SetColor(color);
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

		m_object_laser0->SetVelocity(Vector2(200.0f, 0.0f));

		isLaserUsing = true;
		isLaserLoading = true;

		mouseTargetLaser = mousePosition;
		shootFromShipLocation = m_object_ship0->GetPosition();

		sendLaser = 1;
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

		sendRocket = 1;
	}
}

void Application::UpdateLocalLaser(float deltaTime)
{
	Vector2 direction = mouseTargetLaser - shootFromShipLocation;
	direction.Normalize();
	direction *= laserSpeed;
	m_object_laser0->SetVelocity(direction);

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

	m_object_laser0->Update(deltaTime);
}

void Application::UpdateLocalRocket(float deltaTime)
{
	if (m_object_rocket0->GetPosition().x > m_object_ship1->GetPosition().x)
	{
		m_object_rocket0->SetAcceleration(Vector2(-rocketAccel, m_object_rocket0->GetAcceleration().y));
	}
	else if (m_object_laser0->GetPosition().x < m_object_ship1->GetPosition().x)
	{
		m_object_rocket0->SetAcceleration(Vector2(rocketAccel, m_object_rocket0->GetAcceleration().y));
	}
	if (m_object_rocket0->GetPosition().y > m_object_ship1->GetPosition().y)
	{
		m_object_rocket0->SetAcceleration(Vector2(m_object_rocket0->GetAcceleration().x, -rocketAccel));
	}
	else if (m_object_rocket0->GetPosition().y < m_object_ship1->GetPosition().y)
	{
		m_object_rocket0->SetAcceleration(Vector2(m_object_rocket0->GetAcceleration().y, rocketAccel));
	}

	m_object_rocket0->SetRotation
	(
		CalculatePointRotation
		(
			m_object_rocket0->GetPosition(), m_object_ship1->GetPosition()
		)
	);

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

	m_object_rocket0->Update(deltaTime);
	LimitVelAndPos(m_object_rocket0, rocketSpeed);
}

void Application::UpdateLocalShip(float deltaTime)
{
	if (isImmune)
	{
		immuneDelayCounter += deltaTime;
		if (immuneDelayCounter > immuneDelay)
		{
			immuneDelayCounter = 0.0f;
			isImmune = false;

			m_object_ship0->GetSprite().SetColor(normalColor);
		}
	}

	m_object_ship0->SetRotation
	(
		CalculatePointRotation
		(
			m_object_ship0->GetPosition(), mousePosition
		)
	);

	m_object_ship0->Update(deltaTime);
	m_object_ship0->SetAcceleration(Vector2(0.0f, 0.0f));
	LimitVelAndPos(m_object_ship0, maxShipSpeed);
}

void Application::UpdateLocalShipHealth()
{
	if (playerNumber == 1)
	{
		// Remove them, then update them.
		for (int i = 0; i < maxHealthShip0; ++i)
		{
			m_object_health0[i]->SetPosition(Vector2(-200.0f, -200.0f));
		}

		for (int i = 0; i < healthShip0; ++i)
		{
			m_object_health0[i]->SetPosition(Vector2(60.0f + (i * 60.0f), 540.0f));
		}
	}
	else if (playerNumber == 2)
	{
		// Remove them, then update them.
		for (int i = 0; i < maxHealthShip0; ++i)
		{
			m_object_health0[i]->SetPosition(Vector2(-200.0f, -200.0f));
		}

		for (int i = 0; i < healthShip0; ++i)
		{
			m_object_health0[i]->SetPosition(Vector2(740.0f + (i * -60.0f), 540.0f));
		}
	}
}

void Application::UpdateRemoteLaser(float deltaTime)
{
	// Remote laser.
	m_object_laser1->SetPosition
	(
		m_object_laser1->GetPosition() * 0.995f + m_lastReceivedPos_laser1 * 0.005f
	);

	m_object_laser1->Update(deltaTime);
	m_object_laser1->SetRotation(m_lastReceivedRot_laser1);
}

void Application::UpdateRemoteRocket(float deltaTime)
{
	// Remote rocket.
	m_object_rocket1->SetPosition
	(
		m_object_rocket1->GetPosition() * 0.995f + m_lastReceivedPos_rocket1 * 0.005f
	);

	m_object_rocket1->Update(deltaTime);
	m_object_rocket1->SetRotation(m_lastReceivedRot_rocket1);
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
	LimitVelAndPos(m_object_ship1, maxShipSpeed);
	DamageBlink(m_object_ship1, returnColor);
}

void Application::UpdateRemoteShipHealth()
{
	if (playerNumber == 1)
	{
		// Remove them, then update them.
		for (int i = 0; i < maxHealthShip1; ++i)
		{
			m_object_health1[i]->SetPosition(Vector2(-200.0f, -200.0f));
		}

		for (int i = 0; i < healthShip1; ++i)
		{
			m_object_health1[i]->SetPosition(Vector2(740.0f + (i * -60.0f), 540.0f));
		}
	}
	else if (playerNumber == 2)
	{
		// Remove them, then update them.
		for (int i = 0; i < maxHealthShip1; ++i)
		{
			m_object_health1[i]->SetPosition(Vector2(-200.0f, -200.0f));
		}

		for (int i = 0; i < healthShip1; ++i)
		{
			m_object_health1[i]->SetPosition(Vector2(60.0f + (i * 60.0f), 540.0f));
		}
	}
}

void Application::CheckWinLose()
{
	if (m_gameState == GameState::STATE_STARTGAME)
	{
		if (healthShip0 <= 0)
		{
			m_object_winLose->SetSprite(m_sprite_lose);
			m_object_winLose->SetPosition(Vector2(400.0f, 300.0f));
			m_gameState = GameState::STATE_GAMEOVER;
		}
		else if (healthShip1 <= 0)
		{
			m_object_winLose->SetSprite(m_sprite_win);
			m_object_winLose->SetPosition(Vector2(400.0f, 300.0f));
			m_gameState = GameState::STATE_GAMEOVER;
		}
	}
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
	float rot_ship0 = m_object_ship0->GetRotation();
	float rot_laser0 = m_object_laser0->GetRotation();
	float rot_rocket0 = m_object_rocket0->GetRotation();
	Color hurt_color0 = m_object_ship0->GetSprite().GetColor();
	int hp_ship0 = healthShip0;
	int send_laser = sendLaser;
	int send_rocket = sendRocket;

	MyPhoton::getInstance().sendMyData
	(
		pos_ship0, vel_ship0, acc_ship0,
		pos_laser0, vel_laser0, acc_laser0,
		pos_rocket0, vel_rocket0, acc_rocket0,
		rot_ship0, rot_laser0, rot_rocket0,
		hurt_color0, hp_ship0,
		send_laser, send_rocket
	);
}

// Things to update...
// Ship position, ship rotation, laser position, laser rotation, ship health
void Application::OnReceivedOpponentData(float* data)
{
	if (m_gameState != GameState::STATE_STARTGAME)
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

		return;
	}

	// ship : 0 ~ 5, laser0 : 6 ~ 11, rocket0 : 12 ~ 17,
	// shipRot : 18, laserRot : 19, rocketRot : 20,
	// returnColor.R : 21, G : 22, B : 23, A : 24, shipHealth : 25

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
	
	m_lastReceivedRot_ship1 = data[18];
	m_lastReceivedRot_laser1 = data[19];
	m_lastReceivedRot_rocket1 = data[20];

	returnColor.R = data[21];
	returnColor.G = data[22];
	returnColor.B = data[23];
	returnColor.A = data[24];

	healthShip1 = data[25];

	returnLaser = data[26];
	returnRocket = data[27];
}

void Application::OnKeyPressed(int key)
{
	if (m_gameState != GameState::STATE_STARTGAME)
	{
		return;
	}
}

void Application::OnKeyReleased(int key)
{
	if (m_gameState != GameState::STATE_STARTGAME)
	{
		return;
	}
}

void Application::OnKeyHold(int key)
{
	if (m_gameState != GameState::STATE_STARTGAME)
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
	if (m_gameState != GameState::STATE_STARTGAME)
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
	if (m_gameState != GameState::STATE_STARTGAME)
	{
		return;
	}
}

void Application::OnMouseHold(int button)
{
	if (m_gameState != GameState::STATE_STARTGAME)
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
	if (m_gameState != GameState::STATE_STARTGAME)
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

	if (m_gameState != GameState::STATE_STARTGAME)
	{
		return;
	}

	// Do this before updating remote objects.
	NetworkUpdate();

	UpdateObjectCollision();
	UpdateLocalShip(elapsedTime);
	UpdateLocalLaser(elapsedTime);
	UpdateLocalRocket(elapsedTime);
	UpdateLocalShipHealth();
	UpdateRemoteShip(elapsedTime);
	UpdateRemoteLaser(elapsedTime);
	UpdateRemoteRocket(elapsedTime);
	UpdateRemoteShipHealth();
	
	CheckWinLose();
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
