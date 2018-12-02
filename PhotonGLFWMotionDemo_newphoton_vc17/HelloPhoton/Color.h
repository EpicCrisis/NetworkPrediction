
#pragma once

#include <GLFW/glfw3.h>

struct Color
{
	GLubyte R, G, B, A;

	Color()
	{
		R = 255;
		G = 255;
		B = 255;
		A = 255;
	}

	Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		this->R = r;
		this->G = g;
		this->B = b;
		this->A = a;
	}
};

