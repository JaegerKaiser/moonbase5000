#pragma once
#include "ModelAsset.h"
#include <glm/glm.hpp>

class Quad
{
	static float *AddVertex(float *mem, const glm::vec3& r, const glm::vec3 &n, float u, float v)
	{
		*mem++ = r.x; *mem++ = r.y; *mem++ = r.z;
		*mem++ = u; *mem++ = v;
		*mem++ = n.x; *mem++ = n.y; *mem++ = n.z;
		return mem;
	}

public:
	static ModelAsset *Generate(float width, float height, float uvFac, const char *texture, const char *vProg, const char *fProg)
	{
		// makes a quad, centered on 0,0,0. width is in x direction, height in y direction
		// texture is repreated uvFac times across the quad in both directions

		float *vertexData = new float[48];
		float *v = vertexData;

		glm::vec3 pos[4], uv[4];
		glm::vec3 norm = glm::vec3(0.0f, 0.0f, 1.0);
		pos[0] = glm::vec3(-width*0.5f, -height*0.5f, 0.0);
		pos[1] = glm::vec3(-width*0.5f, height*0.5f, 0.0);
		pos[2] = glm::vec3(width*0.5f, height*0.5f, 0.0);
		pos[3] = glm::vec3(width*0.5f, -height*0.5f, 0.0);
		uv[0] = glm::vec3(0.0f, 0.0f, 0.0);
		uv[1] = glm::vec3(0.0f, uvFac, 0.0);
		uv[2] = glm::vec3(uvFac, uvFac, 0.0);
		uv[3] = glm::vec3(uvFac, 0.0f, 0.0);


		// triangle 0, 1, 2
		v = AddVertex(v, pos[0], norm, uv[0].x, uv[0].y);
		v = AddVertex(v, pos[1], norm, uv[1].x, uv[1].y);
		v = AddVertex(v, pos[2], norm, uv[2].x, uv[2].y);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0], norm, uv[0].x, uv[0].y);
		v = AddVertex(v, pos[2], norm, uv[2].x, uv[2].y);
		v = AddVertex(v, pos[3], norm, uv[3].x, uv[3].y);
	
		ModelAsset *pAsset = new ModelAsset;
		pAsset->LoadShaders(vProg, fProg);
		pAsset->LoadTexture(texture);
		pAsset->SetVertexData(vertexData, 6, 8);
		delete vertexData;
		return pAsset;
	}
}; 
