#include "Application.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "MyPhoton.h"

using namespace std;

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

void Application::start()
{
	srand(time(0));

	m_gameStarted = false;

	MyPhoton::getInstance().connect();

	m_sprite_ship_red		= new Sprite("../media/Spaceship_Red.bmp");
	m_sprite_laser_red		= new Sprite("../media/Laser_Red.bmp");
	m_sprite_rocket_red		= new Sprite("../media/Rocket_Red.bmp");
	m_sprite_ship_blue		= new Sprite("../media/Spaceship_Blue.bmp");
	m_sprite_laser_blue		= new Sprite("../media/Laser_Blue.bmp");
	m_sprite_rocket_blue	= new Sprite("../media/Rocket_Blue.bmp");

	m_object_ship0.setSprite(m_sprite_ship_red);
	m_object_ship0.setPos(Vector(100.0f, 300.0f, 0.0f));
	m_object_ship0.setRotation(0.0f);
	m_object_ship0.setBlendMode(ADDITIVE);

	m_object_ship1.setSprite(m_sprite_ship_blue);
	m_object_ship1.setPos(Vector(100.0f, 300.0f, 0.0f));
	m_object_ship1.setRotation(0.0f);
	m_object_ship1.setBlendMode(ADDITIVE);

	m_lastReceivedPos = m_object_ship1.getPos();
}

void Application::SetPlayerNumber(int playerN)
{
	if (playerNumber == 0)
	{
		playerNumber = playerN;
		std::cout << playerNumber << std::endl;
	}
}

void Application::CheckPlayerShipColour()
{
	if (playerNumber == 1)
	{
		m_object_ship0.setSprite(m_sprite_ship_red);
		m_object_ship1.setSprite(m_sprite_ship_blue);
	}
	else if (playerNumber == 2)
	{
		m_object_ship0.setSprite(m_sprite_ship_blue);
		m_object_ship1.setSprite(m_sprite_ship_red);
	}
}

void Application::sendMyData(void)
{
	Vector pos = m_object_ship0.getPos();
	Vector vel = m_object_ship0.getVelocity();
	Vector acc = m_object_ship0.getAcceleration();

	MyPhoton::getInstance().sendMyData
	(
		pos.mVal[0], pos.mVal[1],
		vel.mVal[0], vel.mVal[1],
		acc.mVal[0], acc.mVal[1]
	);
}

void Application::networkUpdate()
{
	static double prevTime = glfwGetTime();

	double time = glfwGetTime();
	if (time - prevTime >= gNetworkFrameTime)
	{
		sendMyData();
		prevTime = time;
	}
}

void Application::limitVelAndPos(GameObject* go)
{
	if (go->getVelocity().length() > 200.0f)
	{
		Vector vec = go->getVelocity();
		vec.normalize();
		vec *= 200.0f;
		go->setVelocity(vec);
	}
}

void Application::update(double elapsedTime)
{
	MyPhoton::getInstance().run();

	if (!m_gameStarted)
	{
		return;
	}

	CheckPlayerShipColour();

	m_object_ship0.update(elapsedTime);
	m_object_ship0.setAcceleration(Vector(0.0f, 0.0f, 0.0f));
	limitVelAndPos(&m_object_ship0);

	networkUpdate();

	// update remote ship.
	m_object_ship1.update(elapsedTime);
	// very slowly interpolate from on-going predicting pos to lastest received pos. 
	// Without this interpolation, the offset of opponent position will keep being accumulated. 
	m_object_ship1.setPos(m_object_ship1.getPos()*0.995f + m_lastReceivedPos * 0.005f);
	limitVelAndPos(&m_object_ship1);
}

void Application::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!m_gameStarted)
	{
		return;
	}

	m_object_ship0.draw();
	m_object_ship1.draw();
}


void Application::OnReceivedOpponentData(float* data)
{
	if (m_gameStarted == false)
	{
		m_gameStarted = true;
		m_object_ship1.setPos(Vector(data[0], data[1], 0));

		m_lastReceivedPos = m_object_ship1.getPos();
		m_prevReceivedTime = glfwGetTime();
		return;
	}

	// remote ship data.
	m_lastReceivedPos = Vector(data[0], data[1], 0);
	m_object_ship1.setVelocity(Vector(data[2], data[3], 0));
	m_object_ship1.setAcceleration(Vector(data[4], data[5], 0));
}


void Application::onKeyPressed(int key)
{
}

void Application::onKeyReleased(int key)
{
}

void Application::onKeyHold(int key)
{
	if (!m_gameStarted)
	{
		return;
	}

	if (key == GLFW_KEY_W)
	{
		m_object_ship0.setAcceleration(m_object_ship0.getAcceleration() + Vector(0.0f, 200.0f, 0.0f));
	}
	if (key == GLFW_KEY_A)
	{
		m_object_ship0.setAcceleration(m_object_ship0.getAcceleration() + Vector(-200.0f, 0.0f, 0.0f));
	}
	if (key == GLFW_KEY_S)
	{
		m_object_ship0.setAcceleration(m_object_ship0.getAcceleration() + Vector(0.0f, -200.0f, 0.0f));
	}
	if (key == GLFW_KEY_D)
	{
		m_object_ship0.setAcceleration(m_object_ship0.getAcceleration() + Vector(200.0f, 0.0f, 0.0f));
	}
}

void Application::onMousePressed(int button)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_2)
	{

	}
}

void Application::onMouseReleased(int button)
{
}

void Application::onMouseHold(int button)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_2)
	{

	}
}

void Application::onMouseMoved(double mousePosX, double mousePosY)
{
	// Here should have an update that allows the head of the ship to look at the mouse position.
	// Calculate rotation based on mouse position.
}
