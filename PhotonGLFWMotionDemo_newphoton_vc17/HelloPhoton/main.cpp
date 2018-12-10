
#include <iostream>
#include <windows.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include "Application.h"
#include <set>
#include <time.h>

using namespace std;

Application& app = Application::getInstance();
set <int> gDownKeys;
set <int> gDownMouseButtons;

void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//! Use ortho2D view
	gluOrtho2D(0, width, 0, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		app.OnKeyPressed(key);
		gDownKeys.insert(key);
	}
	else if (action == GLFW_RELEASE)
	{
		app.OnKeyReleased(key);
		gDownKeys.erase(key);
	}
}

static void keyHoldCallback()
{
	for (set <int>::iterator iter = gDownKeys.begin(); iter != gDownKeys.end(); ++iter)
	{
		app.OnKeyHold(*iter);
	}
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		app.OnMousePressed(button);
		gDownMouseButtons.insert(button);
	}
	else if (action == GLFW_RELEASE)
	{
		app.OnMouseReleased(button);
		gDownMouseButtons.erase(button);
	}
}

//! Called in main function
static void mouseButtonHoldCallback()
{
	for (set <int>::iterator iter = gDownMouseButtons.begin(); iter != gDownMouseButtons.end(); ++iter)
	{
		app.OnMouseHold(*iter);
	}
}

static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	app.OnMouseMoved(xPos, yPos);
}

int main(void)
{
	GLFWwindow* window;

	srand(time(NULL));

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create m_a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "Network Prediction", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	onWindowResized(window, RESOLUTION_X, RESOLUTION_Y);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	double prevTime = glfwGetTime();
	app.Start();

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		double elapsedTime = currentTime - prevTime;

		app.Update(elapsedTime);
		app.Draw();

		keyHoldCallback();
		mouseButtonHoldCallback();

		glfwSwapBuffers(window);
		glfwPollEvents();

		prevTime = currentTime;
	}

	glfwTerminate();
	return 0;
}