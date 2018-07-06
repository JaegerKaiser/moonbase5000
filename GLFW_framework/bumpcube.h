#pragma once
#include "ModelAsset.h"
#include <glm/glm.hpp>

class BumpCube
{
	static float *AddVertex(float *mem, const glm::vec3& r, const glm::vec3 &n, const glm::vec3 &t, float u, float v)
	{
		*mem++ = r.x; *mem++ = r.y; *mem++ = r.z;
		*mem++ = u; *mem++ = v;
		*mem++ = n.x; *mem++ = n.y; *mem++ = n.z;
		*mem++ = t.x; *mem++ = t.y; *mem++ = t.z;
		return mem;
	}

public:
	static ModelAsset *Generate(float size, float uvFac, const char *texture, const char *bumpMap, const char *vProg, const char *fProg)
	{
		// makes an axis-aligned cube, centered on 0,0,0. size is the width/height/depth.
		// texture is repreated uvFac times across each face

		float *vertexData = new float[36 * 11];
		float *v = vertexData;

		glm::vec3 pos[4], uv[4], norm, tangent;
		uv[0] = glm::vec3(0.0f, 0.0f, 0.0);
		uv[1] = glm::vec3(0.0f, uvFac, 0.0);
		uv[2] = glm::vec3(uvFac, uvFac, 0.0);
		uv[3] = glm::vec3(uvFac, 0.0f, 0.0);

		// +z face
		norm = glm::vec3(0.0f, 0.0f, 1.0);
		tangent = glm::vec3(1.0f, 0.0f, 0.0f);
		pos[0] = glm::vec3(-size*0.5f, -size*0.5f, size*0.5f);
		pos[1] = glm::vec3(-size*0.5f, size*0.5f, size*0.5f);
		pos[2] = glm::vec3(size*0.5f, size*0.5f, size*0.5f);
		pos[3] = glm::vec3(size*0.5f, -size*0.5f, size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0], norm, tangent, uv[0].x, uv[0].y);
		v = AddVertex(v, pos[1], norm, tangent, uv[1].x, uv[1].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[2].x, uv[2].y);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0], norm, tangent, uv[0].x, uv[0].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[2].x, uv[2].y);
		v = AddVertex(v, pos[3], norm, tangent, uv[3].x, uv[3].y);

		// -z face
		norm = glm::vec3(0.0f, 0.0f, -1.0);
		tangent = glm::vec3(-1.0f, 0.0f, 0.0f);
		pos[0] = glm::vec3(-size*0.5f, -size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(-size*0.5f, size*0.5f, -size*0.5f);
		pos[2] = glm::vec3(size*0.5f, size*0.5f, -size*0.5f);
		pos[3] = glm::vec3(size*0.5f, -size*0.5f, -size*0.5f);
		// on '-' faces, we change order of uv coords to reverse the image
		// triangle 0, 1, 2
		v = AddVertex(v, pos[0], norm, tangent, uv[3].x, uv[3].y);
		v = AddVertex(v, pos[1], norm, tangent, uv[2].x, uv[2].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[1].x, uv[1].y);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0], norm, tangent, uv[3].x, uv[3].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[1].x, uv[1].y);
		v = AddVertex(v, pos[3], norm, tangent, uv[0].x, uv[0].y);
		// +x face
		norm = glm::vec3(1.0f, 0.0f, 0.0);
		tangent = glm::vec3(0.0f, 1.0f, 0.0f);
		pos[0] = glm::vec3(size*0.5f, -size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(size*0.5f, -size*0.5f, size*0.5f);
		pos[2] = glm::vec3(size*0.5f, size*0.5f, size*0.5f);
		pos[3] = glm::vec3(size*0.5f, size*0.5f, -size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0], norm, tangent, uv[0].x, uv[0].y);
		v = AddVertex(v, pos[1], norm, tangent, uv[1].x, uv[1].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[2].x, uv[2].y);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0], norm, tangent, uv[0].x, uv[0].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[2].x, uv[2].y);
		v = AddVertex(v, pos[3], norm, tangent, uv[3].x, uv[3].y);
		// -x face
		norm = glm::vec3(-1.0f, 0.0f, 0.0);
		tangent = glm::vec3(0.0f, -1.0f, 0.0f);
		pos[0] = glm::vec3(-size*0.5f, -size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(-size*0.5f, -size*0.5f, size*0.5f);
		pos[2] = glm::vec3(-size*0.5f, size*0.5f, size*0.5f);
		pos[3] = glm::vec3(-size*0.5f, size*0.5f, -size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0], norm, tangent, uv[3].x, uv[3].y);
		v = AddVertex(v, pos[1], norm, tangent, uv[2].x, uv[2].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[1].x, uv[1].y);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0], norm, tangent, uv[3].x, uv[3].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[1].x, uv[1].y);
		v = AddVertex(v, pos[3], norm, tangent, uv[0].x, uv[0].y);
		// +y face
		norm = glm::vec3(0.0f, 1.0f, 0.0);
		tangent = glm::vec3(0.0f, 0.0f, 1.0f);
		pos[0] = glm::vec3(-size*0.5f, size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(size*0.5f, size*0.5f, -size*0.5f);
		pos[2] = glm::vec3(size*0.5f, size*0.5f, size*0.5f);
		pos[3] = glm::vec3(-size*0.5f, size*0.5f, size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0], norm, tangent, uv[0].x, uv[0].y);
		v = AddVertex(v, pos[1], norm, tangent, uv[1].x, uv[1].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[2].x, uv[2].y);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0], norm, tangent, uv[0].x, uv[0].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[2].x, uv[2].y);
		v = AddVertex(v, pos[3], norm, tangent, uv[3].x, uv[3].y);

		// -y face
		norm = glm::vec3(0.0f, -1.0f, 0.0);
		tangent = glm::vec3(0.0f, 0.0f, -1.0f);
		pos[0] = glm::vec3(-size*0.5f, -size*0.5f, -size*0.5f);
		pos[1] = glm::vec3(size*0.5f, -size*0.5f, -size*0.5f);
		pos[2] = glm::vec3(size*0.5f, -size*0.5f, size*0.5f);
		pos[3] = glm::vec3(-size*0.5f, -size*0.5f, size*0.5f);

		// triangle 0, 1, 2
		v = AddVertex(v, pos[0], norm, tangent, uv[3].x, uv[3].y);
		v = AddVertex(v, pos[1], norm, tangent, uv[2].x, uv[2].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[1].x, uv[1].y);
		// triangle 0, 2, 3
		v = AddVertex(v, pos[0], norm, tangent, uv[3].x, uv[3].y);
		v = AddVertex(v, pos[2], norm, tangent, uv[1].x, uv[1].y);
		v = AddVertex(v, pos[3], norm, tangent, uv[0].x, uv[0].y);
		ModelAsset *pAsset = new ModelAsset;
		pAsset->LoadShaders(vProg, fProg);
		pAsset->LoadTexture(texture);
		pAsset->LoadBumpMap(bumpMap);
		pAsset->SetVertexData(vertexData, 36, 11);
		delete vertexData;
		return pAsset;
	}
};
