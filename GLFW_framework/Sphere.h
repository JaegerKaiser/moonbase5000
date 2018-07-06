#pragma once
#include "ModelAsset.h"
#define RADIANFAC (1.0f/57.2958f)

class Sphere
{
	static float *AddVertex(float *mem, const glm::vec3& r, const glm::vec3 &n, float u, float v)
	{
		*mem++ = r.x; *mem++ = r.y; *mem++ = r.z;
		*mem++ = u; *mem++ = v;
		*mem++ = n.x; *mem++ = n.y; *mem++ = n.z;
		return mem;
	}
public:
	static ModelAsset *Generate(float radius, const char *texture, const char *vProg, const char *fProg, int nTess )
	{
		// points on a sphere - square grid of latitude/longitude
		glm::vec3 *points = new glm::vec3[nTess * nTess];

		for (int i = 0; i < nTess; ++i)
		{
			float phi = RADIANFAC * 360.0f * (float)i / (float)(nTess-1);
			for (int j = 0; j < nTess; ++j)
			{
				float theta = RADIANFAC * (-90.0f + 180.0f * (float)j / (float)(nTess-1) );
				float x, y, z;

				y = sin(theta);
				x = cos(theta)*cos(phi);
				z = cos(theta)*sin(phi);
				points[i*nTess + j] = glm::vec3(x, y, z);
			}
		}
		// make the vertex data
		// nTess-1 x nTess-1 squares, each with 6 verts, 8 floats per vertex
		float *vertexData = new float[(nTess - 1)*(nTess - 1) * 8 * 6];
		float *v = vertexData;
		for (int i = 0; i < nTess - 1; ++i)
		{
			for (int j = 0; j < nTess - 1; ++j)
			{
				glm::vec3 pos[4], norm[4], uv[4];
				norm[0] = points[i*nTess + j];			pos[0] = norm[0] * radius;
				norm[1] = points[i*nTess + j+1];		pos[1] = norm[1] * radius;
				norm[2] = points[(i+1)*nTess + j+1];	pos[2] = norm[2] * radius;
				norm[3] = points[(i+1)*nTess + j];		pos[3] = norm[3] * radius;
				uv[0] = glm::vec3((float)i / (float)nTess, (float)j / (float)nTess, 0.0f);
				uv[1] = glm::vec3((float)i / (float)nTess, (float)(j+1) / (float)nTess, 0.0f);
				uv[2] = glm::vec3((float)(i+1) / (float)nTess, (float)(j+1) / (float)nTess, 0.0f);
				uv[3] = glm::vec3((float)(i+1) / (float)nTess, (float)j / (float)nTess, 0.0f);
				// triangle 0, 1, 2
				v = AddVertex(v, pos[0], norm[0], uv[0].x, uv[0].y);
				v = AddVertex(v, pos[1], norm[1], uv[1].x, uv[1].y);
				v = AddVertex(v, pos[2], norm[2], uv[2].x, uv[2].y);
				// triangle 0, 2, 3
				v = AddVertex(v, pos[0], norm[0], uv[0].x, uv[0].y);
				v = AddVertex(v, pos[2], norm[2], uv[2].x, uv[2].y);
				v = AddVertex(v, pos[3], norm[3], uv[3].x, uv[3].y);
			}
		}

		ModelAsset *pAsset = new ModelAsset;
		pAsset->LoadShaders(vProg, fProg);
		pAsset->LoadTexture(texture);
		pAsset->SetVertexData( vertexData, (nTess - 1)*(nTess - 1) * 6, 8);
		delete vertexData;
		delete points;
		return pAsset;
	}
};