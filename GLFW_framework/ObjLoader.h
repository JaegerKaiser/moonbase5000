#pragma once
#include "ModelAsset.h"
#include "tiny_obj_loader.h"

class ObjLoader
{
	static float *AddVertex(float *mem, const glm::vec3& r, const glm::vec3 &n, float u, float v)
	{
		*mem++ = r.x; *mem++ = r.y; *mem++ = r.z;
		*mem++ = u; *mem++ = v;
		*mem++ = n.x; *mem++ = n.y; *mem++ = n.z;
		return mem;
	}

public:
	static ModelAsset *Load(const char *directory, const char *modelFile, const char *vShader, const char *fShader)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		std::string modelName(directory);
		modelName += std::string(modelFile);
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelName.c_str(), directory, true))
		{
			throw std::runtime_error(err);
		}
		int numVerts = shapes[0].mesh.indices.size();
		float *vertexData = new float[numVerts * 8];
		float *vMem = vertexData;
		glm::vec3 pos, norm;
		glm::vec2 uv;
		for (int i = 0; i < numVerts; i++)
		{
			int ivert, inorm, itex;
			ivert = shapes[0].mesh.indices[i].vertex_index;
			inorm = shapes[0].mesh.indices[i].normal_index;
			itex = shapes[0].mesh.indices[i].texcoord_index;

			pos.x = attrib.vertices[ivert * 3 + 0];
			pos.y = attrib.vertices[ivert * 3 + 1];
			pos.z = attrib.vertices[ivert * 3 + 2];

			norm.x = attrib.normals[inorm * 3 + 0];
			norm.y = attrib.normals[inorm * 3 + 1];
			norm.z = attrib.normals[inorm * 3 + 2];

			uv.x = attrib.texcoords[itex * 2 + 0];
			uv.y = attrib.texcoords[itex * 2 + 1];
			vMem = AddVertex(vMem, pos, norm, uv.x, uv.y);
		}
		ModelAsset *pAsset = new ModelAsset;
		pAsset->LoadShaders(vShader, fShader);
		std::string texName = std::string(directory);
		texName += std::string(materials[0].diffuse_texname);
		pAsset->LoadTexture(texName.c_str());
		pAsset->SetVertexData(vertexData, numVerts, 8);
		delete vertexData;
		return pAsset;
	}
};
