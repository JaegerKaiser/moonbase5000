#include "ModelAsset.h"

void ModelAsset::LoadShaders(const char *v_shader, const char *f_shader)
{
	std::vector<tdogl::Shader> vs;
	vs.push_back(tdogl::Shader::shaderFromFile(v_shader, GL_VERTEX_SHADER));
	vs.push_back(tdogl::Shader::shaderFromFile(f_shader, GL_FRAGMENT_SHADER));
	shaders = new tdogl::Program(vs);

}

void ModelAsset::LoadTexture(const char *fileName)
{
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(fileName);
	bmp.flipVertically();
	texture = new tdogl::Texture(bmp);
	shininess = 80.0;
	specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

void ModelAsset::LoadBumpMap(const char *fileName)
{
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(fileName);
	bmp.flipVertically();
	bumpMap = new tdogl::Texture(bmp);
}


void ModelAsset::SetVertexData(float *vData, int numVerts, int vertSize)
{
	drawType = GL_TRIANGLES;
	drawStart = 0;
	drawCount = numVerts * 3;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind the VAO
	glBindVertexArray(vao);

	// bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertSize*numVerts, vData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(shaders->attrib("vert"));
	glVertexAttribPointer(shaders->attrib("vert"), 3, GL_FLOAT, GL_FALSE, vertSize * sizeof(GLfloat), (const GLvoid*)0);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(shaders->attrib("vertTexCoord"));
	glVertexAttribPointer(shaders->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE, vertSize * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	// connect the normal to the "vertNormal" attribute of the vertex shader
	glEnableVertexAttribArray(shaders->attrib("vertNormal"));
	glVertexAttribPointer(shaders->attrib("vertNormal"), 3, GL_FLOAT, GL_TRUE, vertSize * sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

	if (vertSize > 8)
	{
		// connect the tangent vector to the "vertTangent" attribute of the vertex shader
		glEnableVertexAttribArray(shaders->attrib("vertTangent"));
		glVertexAttribPointer(shaders->attrib("vertTangent"), 3, GL_FLOAT, GL_TRUE,
			vertSize * sizeof(GLfloat), (const GLvoid*)(8 * sizeof(GLfloat)));
	}
	// unbind the VAO
	glBindVertexArray(0);
}

