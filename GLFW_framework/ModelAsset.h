#pragma once

#include "Program.h"
#include "Texture.h"

class ModelAsset
{
	/*
	Represents a textured geometry asset

	Contains everything necessary to draw arbitrary geometry with a single texture:

	- shaders
	- a texture
	- a VBO
	- a VAO
	- the parameters to glDrawArrays (drawType, drawStart, drawCount)
	*/
	friend class ModelInstance;
private:
	tdogl::Program* shaders;
	tdogl::Texture* texture;
	tdogl::Texture* bumpMap;
	GLuint vbo;
	GLuint vao;
	GLenum drawType;
	GLint drawStart;
	GLint drawCount;
	GLfloat shininess;
	glm::vec3 specularColor;

public:
	ModelAsset() :
		shaders(NULL),
		texture(NULL),
		bumpMap(NULL),
		vbo(0),
		vao(0),
		drawType(GL_TRIANGLES),
		drawStart(0),
		drawCount(0),
		shininess(0.0f),
		specularColor(1.0f, 1.0f, 1.0f)
	{}

	void LoadShaders(const char *v_shader, const char *f_shader);
	void LoadTexture(const char *fileName);
	void LoadBumpMap(const char *fileName);
	void SetVertexData(float *vData, int numVerts, int vertSize);
};