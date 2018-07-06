#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "CubeMap.h"
#include "Program.h"

class SkyBox
{
	tdogl::Program* shaders;
	tdogl::CubeMap* cubeMap;
	GLuint vbo;
	GLuint vao;
	GLenum drawType;
	GLint drawStart;
	GLint drawCount;

	static float *AddVertex(float *mem, const glm::vec3& r)
	{
		*mem++ = r.x; *mem++ = r.y; *mem++ = r.z;
		return mem;
	}

public:
	SkyBox( std::string cubeMaps[], const char *vProg, const char *fProg)
	{
		//load the shaders
		std::vector<tdogl::Shader> vs;
		vs.push_back(tdogl::Shader::shaderFromFile(vProg, GL_VERTEX_SHADER));
		vs.push_back(tdogl::Shader::shaderFromFile(fProg, GL_FRAGMENT_SHADER));
		shaders = new tdogl::Program(vs);


		float *vertexData = new float[6 * 6 * 3]; // vertex only has position - no uv or normal
		float *v = vertexData;

		glm::vec3 pos[4];
		float size = 10.0f;

		// +z face
		pos[0] = glm::vec3(-size*0.5f, -size*0.5f, size*0.5f);
		pos[1] = glm::vec3(-size*0.5f, size*0.5f, size*0.5f);
		pos[2] = glm::vec3(size*0.5f, size*0.5f, size*0.5f);
		pos[3] = glm::vec3(size*0.5f, -size*0.5f, size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[1]);
		v = AddVertex(v, pos[2]);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[2]);
		v = AddVertex(v, pos[3]);

		// -z face
		pos[0] = glm::vec3(-size*0.5f, -size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(-size*0.5f, size*0.5f, -size*0.5f);
		pos[2] = glm::vec3(size*0.5f, size*0.5f, -size*0.5f);
		pos[3] = glm::vec3(size*0.5f, -size*0.5f, -size*0.5f);
		// triangle 0, 1, 2
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[1]);
		v = AddVertex(v, pos[2]);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[2]);
		v = AddVertex(v, pos[3]);

		// +x face
		pos[0] = glm::vec3(size*0.5f, -size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(size*0.5f, -size*0.5f, size*0.5f);
		pos[2] = glm::vec3(size*0.5f, size*0.5f, size*0.5f);
		pos[3] = glm::vec3(size*0.5f, size*0.5f, -size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[1]);
		v = AddVertex(v, pos[2]);
		// triangle 0, 2, 3	    
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[2]);
		v = AddVertex(v, pos[3]);
		// -x face
		pos[0] = glm::vec3(-size*0.5f, -size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(-size*0.5f, -size*0.5f, size*0.5f);
		pos[2] = glm::vec3(-size*0.5f, size*0.5f, size*0.5f);
		pos[3] = glm::vec3(-size*0.5f, size*0.5f, -size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[1]);
		v = AddVertex(v, pos[2]);
		// triangle 0, 2, 3	    
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[2]);
		v = AddVertex(v, pos[3]);
		// +y face
		pos[0] = glm::vec3(-size*0.5f, size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(size*0.5f, size*0.5f, -size*0.5f);
		pos[2] = glm::vec3(size*0.5f, size*0.5f, size*0.5f);
		pos[3] = glm::vec3(-size*0.5f, size*0.5f, size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[1]);
		v = AddVertex(v, pos[2]);
		// triangle 0, 2, 3	    
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[2]);
		v = AddVertex(v, pos[3]);

		// -y face
		pos[0] = glm::vec3(-size*0.5f, -size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(size*0.5f, -size*0.5f, -size*0.5f);
		pos[2] = glm::vec3(size*0.5f, -size*0.5f, size*0.5f);
		pos[3] = glm::vec3(-size*0.5f, -size*0.5f, size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[1]);
		v = AddVertex(v, pos[2]);
		// triangle 0, 2, 3	    
		v = AddVertex(v, pos[0]);
		v = AddVertex(v, pos[2]);
		v = AddVertex(v, pos[3]);

		// create the vertex buffers
		int numVerts = 36;
		drawType = GL_TRIANGLES;
		drawStart = 0;
		drawCount = numVerts * 3;
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);

		// bind the VAO
		glBindVertexArray(vao);

		// 26*bind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*numVerts, vertexData, GL_STATIC_DRAW);

		// connect the xyz to the "vert" attribute of the vertex shader
		glEnableVertexAttribArray(shaders->attrib("vert"));
		glVertexAttribPointer(shaders->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const GLvoid*)0);

		// unbind the VAO
		glBindVertexArray(0);

		// load the cubemap
		tdogl::Bitmap *cubeMapFaces = new tdogl::Bitmap[6];
		for (int i = 0; i < 6; i++)
		{
			cubeMapFaces[i] = tdogl::Bitmap::bitmapFromFile(cubeMaps[i]);

		}
		cubeMap = new tdogl::CubeMap(cubeMapFaces);
	}

	void Render(const tdogl::Camera &camera) const
	{
		// don't write to the z buffer
		glDepthMask(GL_FALSE);
		//bind the shaders
		shaders->use();

		//set the shader uniforms
		shaders->setUniform("camera", camera.matrix());
		shaders->setUniform("cubeMap", 0); //set to 0 because the texture will be bound to GL_TEXTURE0
		shaders->setUniform("cameraPosition", camera.position());

		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->object());

		//bind VAO and draw
		glBindVertexArray(vao);
		glDrawArrays(drawType, drawStart, drawCount);

		//unbind everything
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		shaders->stopUsing();
		// turn the z write back on 
		glDepthMask(GL_TRUE);
	}

	tdogl::CubeMap *GetCubeMap() const { return cubeMap; }
};
