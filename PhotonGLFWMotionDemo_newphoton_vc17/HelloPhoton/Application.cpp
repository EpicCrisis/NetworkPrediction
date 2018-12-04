
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

void Application::Start()
{
	srand(time(0));

	m_gameState = GameState::STATE_WAITGAME;

	MyPhoton::getInstance().connect();

	InitializeSprites();

	InitializeObject();

	m_lastReceivedPos = m_object_ship1->GetTransform().position;
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

void Application::InitializeSprites()
{
	m_sprite_ship_red.SetFilePath("../media/Spaceship_Red.bmp");
	m_sprite_laser_red.SetFilePath("../media/Laser_Red.bmp");
	m_sprite_rocket_red.SetFilePath("../media/Rocket_Red.bmp");
	m_sprite_ship_blue.SetFilePath("../media/Spaceship_Blue.bmp");
	m_sprite_laser_blue.SetFilePath("../media/Laser_Blue.bmp");
	m_sprite_rocket_blue.SetFilePath("../media/Rocket_Blue.bmp");
	m_sprite_asteroid.SetFilePath("../media/Asteroid.bmp");

	m_sprite_ship_red.SetDimension(100.0f, 103.0f);
	m_sprite_laser_red.SetDimension(20.0f, 64.0f);
	m_sprite_rocket_red.SetDimension(20.0f, 80.0f);
	m_sprite_ship_blue.SetDimension(100.0f, 103.0f);
	m_sprite_laser_blue.SetDimension(20.0f, 64.0f);
	m_sprite_rocket_blue.SetDimension(20.0f, 80.0f);
	m_sprite_asteroid.SetDimension(126.0f, 114.0f);
}

void Application::InitializeObject()
{
	m_object_ship0 = Spawn(Vector2(100.0f, 300.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_ship1 = Spawn(Vector2(700.0f, 300.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_ship0->SetSprite(m_sprite_ship_red);
	m_object_ship1->SetSprite(m_sprite_ship_blue);

	m_object_asteroid0 = Spawn(Vector2(200.0f, 500.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_asteroid1 = Spawn(Vector2(600.0f, 100.0f), 0.0f, Vector2(1.0f, 1.0f));
	m_object_asteroid0->SetSprite(m_sprite_asteroid);
	m_object_asteroid1->SetSprite(m_sprite_asteroid);

	m_object_ship0->SetHalfSize(shipHalfSize);
	m_object_ship1->SetHalfSize(shipHalfSize);

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
	}
	else if (playerNumber == 2)
	{
		m_object_ship0->SetSprite(m_sprite_ship_blue);
		m_object_ship1->SetSprite(m_sprite_ship_red);
	}
	m_object_ship0->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
	m_object_ship1->GetSprite().SetBlendingMode(BLEND_ADDITIVE);
}

void Application::UpdateObjectCollision()
{
	if (CheckObjectCollision(m_object_ship0, m_object_asteroid0))
	{

	}
	if (CheckObjectCollision(m_object_ship0, m_object_asteroid1))
	{

	}
	if (CheckObjectCollision(m_object_ship0, m_object_ship1))
	{

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

void Application::SendMyData(void)
{
	Vector2 pos = m_object_ship0->GetTransform().position;
	Vector2 vel = m_object_ship0->GetTransform().velocity;
	Vector2 acc = m_object_ship0->GetTransform().acceleration;

	MyPhoton::getInstance().sendMyData
	(
		pos.x, pos.y,
		vel.x, vel.y,
		acc.x, acc.y
	);
}

void Application::networkUpdate()
{
	static double prevTime = glfwGetTime();

	double time = glfwGetTime();
	if (time - prevTime >= gNetworkFrameTime)
	{
		SendMyData();
		prevTime = time;
	}
}

void Application::limitVelAndPos(GameObject* go)
{
	if (go->GetTransform().velocity.Length() > 200.0f)
	{
		Vector2 vec = go->GetTransform().velocity;
		vec.Normalize();
		vec *= 200.0f;
		go->SetVelocity(vec);
	}
}

void Application::Update(double elapsedTime)
{
	MyPhoton::getInstance().run();

	if (m_gameState == GameState::STATE_WAITGAME)
	{
		return;
	}

	CheckPlayerColour();
	UpdateObjectCollision();

	m_object_ship0->Update(elapsedTime);
	m_object_ship0->SetAcceleration(Vector2(0.0f, 0.0f));
	limitVelAndPos(m_object_ship0);

	networkUpdate();

	// update remote ship.
	m_object_ship1->Update(elapsedTime);
	// very slowly interpolate from on-going predicting pos to lastest received pos. 
	// Without this interpolation, the offset of opponent position will keep being accumulated. 
	m_object_ship1->SetPosition
	(
		m_object_ship1->GetTransform().position * 0.995f + m_lastReceivedPos * 0.005f
	);
	limitVelAndPos(m_object_ship1);
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
		}

		m_gameState = GameState::STATE_STARTGAME;
		m_object_ship1->SetPosition(Vector2(data[0], data[1]));

		m_lastReceivedPos = m_object_ship1->GetTransform().position;
		m_prevReceivedTime = glfwGetTime();
		return;
	}

	// remote ship data.
	m_lastReceivedPos = Vector2(data[0], data[1]);
	m_object_ship1->SetVelocity(Vector2(data[2], data[3]));
	m_object_ship1->SetAcceleration(Vector2(data[4], data[5]));
}

void Application::OnKeyPressed(int key)
{
}

void Application::OnKeyReleased(int key)
{
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
			m_object_ship0->GetTransform().acceleration + Vector2(0.0f, 200.0f)
		);
	}
	if (key == GLFW_KEY_A)
	{
		m_object_ship0->SetAcceleration
		(
			m_object_ship0->GetTransform().acceleration + Vector2(-200.0f, 0.0f)
		);
	}
	if (key == GLFW_KEY_S)
	{
		m_object_ship0->SetAcceleration
		(
			m_object_ship0->GetTransform().acceleration + Vector2(0.0f, -200.0f)
		);
	}
	if (key == GLFW_KEY_D)
	{
		m_object_ship0->SetAcceleration
		(
			m_object_ship0->GetTransform().acceleration + Vector2(200.0f, 0.0f)
		);
	}
}

void Application::OnMousePressed(int button)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_2)
	{

	}
}

void Application::OnMouseReleased(int button)
{
}

void Application::OnMouseHold(int button)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_2)
	{

	}
}

void Application::OnMouseMoved(double mousePosX, double mousePosY)
{
	// Here should have an update that allows the head of the ship to look at the mouse position.
	// Calculate rotation based on mouse position.
}
