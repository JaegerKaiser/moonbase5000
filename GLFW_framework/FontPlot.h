#pragma once
#include "Program.h"
#include "Texture.H"
#include "Font.h"

class FontPlot
{
	tdogl::Program* shaders;
	tdogl::Texture* texture;
	GLuint vbo;
	GLuint vao;
	GLenum drawType;
	GLint drawStart;
	GLint drawCount;
	tdogl::Font *pFont;
	int screenWidth;
	int screenHeight;
	void LoadShaders(const char *v_shader, const char *f_shader);
	void LoadFont(const char *fileName);
	void SetVertexData(float *vData, int numVerts, int vertSize);
public:
	enum Justify
	{
		LEFT=0,
		CENTRE,
		RIGHT
	};
	FontPlot(const char *font, const char *vertexShader, const char *fragmentShader, int width, int height);
	void DrawChar(float x, float y, char c, const glm::vec4 &colour);
	void DrawString(int x, int y, const std::string &str, const glm::vec4 &colour, Justify just=LEFT );
};