#pragma once
#pragma once
#include "ModelAsset.h"
#define RADIANFAC (1.0f/57.2958f)

class Capsule
{
	static float *AddVertex(float *mem, const glm::vec3& r, const glm::vec3 &n, float u, float v)
	{
		*mem++ = r.x; *mem++ = r.y; *mem++ = r.z;
		*mem++ = u; *mem++ = v;
		*mem++ = n.x; *mem++ = n.y; *mem++ = n.z;
		return mem;
	}
public:
	static ModelAsset *Generate(float radius, float length, const char *texture, const char *vProg, const char *fProg, int nTess)
	{
		// points on a sphere - square grid of latitude/longitude
		glm::vec3 *points = new glm::vec3[nTess * nTess];
		glm::vec3 *norms = new glm::vec3[nTess * nTess];
		glm::vec3 *uvs = new glm::vec3[nTess * nTess];
		float distOffset = length / 2;
		float textureOffset = length /(3.1415926f * radius) ;

		for (int i = 0; i < nTess; ++i)
		{
			float phi = RADIANFAC * 360.0f * (float)i / (float)(nTess - 1);
			for (int j = 0; j < nTess; ++j)
			{
				float theta = RADIANFAC * (-90.0f + 180.0f * (float)j / (float)(nTess - 1));
				float x, y, z;

				y = sin(theta);
				x = cos(theta)*cos(phi);
				z = cos(theta)*sin(phi);

				float u, v;
				u = (float)i / (float)nTess;
				v = (float)j / (float)nTess;

				norms[i*nTess + j] = glm::vec3(x, y, z);
				x *= radius;
				y *= radius;
				z *= radius;

				if (theta < 0.0f)
				{
					y -= distOffset;
					v -= textureOffset;
				}
				else
				{
					y += distOffset;
					v += textureOffset;
				}
				points[i*nTess + j] = glm::vec3(x, y, z);
				uvs[i*nTess + j] = glm::vec3(u, v, 0.0f);
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
				norm[0] = norms[i*nTess + j];				
				norm[1] = norms[i*nTess + j + 1];			
				norm[2] = norms[(i + 1)*nTess + j + 1];		
				norm[3] = norms[(i + 1)*nTess + j];			

				pos[0] = points[i*nTess + j];
				pos[1] = points[i*nTess + j + 1];
				pos[2] = points[(i + 1)*nTess + j + 1];
				pos[3] = points[(i + 1)*nTess + j];

				uv[0] = uvs[i*nTess + j];
				uv[1] = uvs[i*nTess + j + 1];
				uv[2] = uvs[(i + 1)*nTess + j + 1];
				uv[3] = uvs[(i + 1)*nTess + j];
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
		pAsset->SetVertexData(vertexData, (nTess - 1)*(nTess - 1) * 6, 8);
		delete vertexData;
		delete points;
		delete norms;
		delete uvs;
		return pAsset;
	}
};