
#include "Sprite.h"

Sprite::Sprite()
{
	m_width = 128;
	m_height = 128;
}

Matrix Sprite::SetTranslate(float x, float y)
{
	return Matrix::makeTranslationMatrix(Vector(x, y, 0.0f));
}

Matrix Sprite::SetRotate(float rotation)
{
	return Matrix::makeRotateMatrix(rotation, Vector(0.0f, 0.0f, 1.0f));
}

Matrix Sprite::SetScale(float x, float y)
{
	return Matrix::makeScaleMatrix(Vector(x, y, 0.0f));
}

Sprite::Sprite(const std::string& path)
{
	SetFilePath(path);
}

Sprite::~Sprite()
{
}

void Sprite::LoadTexture(const char *path, GLuint textureID, unsigned int &width, unsigned int &height)
{
	CBitmap bitmap(path);

	glBindTexture(GL_TEXTURE_2D, textureID);

	// Repeat texture in tiles.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Horizontal texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Vertical texture.

																  // Bilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Near filter.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Far filter.

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(),
		0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());
}

void Sprite::DrawSquare(GLuint textureID, int xPos, int yPos, int width, int height, GLubyte r, GLubyte g, GLubyte b)

{
	float halfW = (float)width * 0.5f;
	float halfH = (float)height * 0.5f;

	GLfloat vertices[] =
	{
		xPos - halfW, yPos - halfH, 0.0f, // bottom left
		xPos - halfW, yPos + halfH, 0.0f, // top left
		xPos + halfW, yPos - halfH, 0.0f, // bottom right

		xPos - halfW, yPos + halfH, 0.0f, // top left
		xPos + halfW, yPos - halfH, 0.0f, // bottom right
		xPos + halfW, yPos + halfH, 0.0f  // top right
	};

	GLfloat texCoords[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	GLubyte colors[] =
	{
		r, g, b,
		r, g, b,
		r, g, b,

		r, g, b,
		r, g, b,
		r, g, b
	};

	// Enable textures.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Sprite::SetFilePath(const std::string & path)
{
	glGenTextures(1, m_textureID);
	LoadTexture(path.c_str(), m_textureID[0], m_width, m_height);
}

void Sprite::SetTextureID(GLuint textureID)
{
	m_textureID[0] = textureID;
}

void Sprite::SetColor(const Color& color)
{
	m_color = color;
}

void Sprite::SetBlendingMode(EnumBlend blend)
{
	m_blendMode = blend;
}

void Sprite::SetDimension(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
}

void Sprite::Draw()
{
	translateMatrix = SetTranslate(m_transform.position.x, m_transform.position.y);
	rotateMatrix = SetRotate(m_transform.rotation);
	scaleMatrix = SetScale(m_transform.scale.x, m_transform.scale.y);

	Matrix viewMatrix = translateMatrix * rotateMatrix * scaleMatrix;

	glLoadMatrixf((GLfloat*)viewMatrix.mVal);

	// enable alpha functions
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

	glDepthMask(true);

	if (m_blendMode == BLEND_ADDITIVE)
	{
		//glBlendFunc(GL_ONE, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else if (m_blendMode == BLEND_MULTIPLY)
	{
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		//glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glDepthMask(false);
	}
	else if (m_blendMode == BLEND_ALPHA)
	{

	}
	else
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	DrawSquare(m_textureID[0], 0, 0, m_width, m_height, GetColor().R, GetColor().G, GetColor().B);

	glDepthMask(true);

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
}

void Sprite::Draw(float xPos, float yPos, float rotation, float xScale, float yScale)
{
	m_transform.position.x = xPos;
	m_transform.position.y = yPos;
	m_transform.rotation = rotation;
	m_transform.scale.x = xScale;
	m_transform.scale.y = yScale;

	Draw();
}

void Sprite::Draw(Vector2 position, float rotation, Vector2 scale)
{
	m_transform.position = position;
	m_transform.rotation = rotation;
	m_transform.scale = scale;

	Draw();
}

void Sprite::Draw(Transform2D transform)
{
	m_transform = transform;

	Draw();
}
