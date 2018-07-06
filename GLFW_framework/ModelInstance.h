#pragma once
#include "ModelAsset.h"
#include "Camera.h"
#include "Light.h"
#include "glm/glm.hpp"
#include "Component.h"
#include "Object.h"

class ModelInstance : public Component 
{
private:
	const ModelAsset &asset;
public:
	ModelInstance(const ModelAsset &asset) : asset(asset) {}
	virtual void Render( const tdogl::Camera &camera, const Light *light, int numLights) const 
	{
		tdogl::Program* shaders = asset.shaders;

		//bind the shaders
		shaders->use();

		//set the shader uniforms
		shaders->setUniform("camera", camera.matrix());
		shaders->setUniform("model", pParent->GetTransform() );
		shaders->setUniform("materialTex", 0); //set to 0 because the texture will be bound to GL_TEXTURE0
		shaders->setUniform("materialShininess", asset.shininess);
		shaders->setUniform("materialSpecularColor", asset.specularColor);
		vec3 intensities[4] = { vec3(0.0f,0.0f,0.0f) };
		vec3 position[4] = { vec3(0.0f, 0.0f, 0.0f) };
		float attenuation[4] = { 0.0f };
		float ambientCoefficient[4] = { 0.0f };
		for (int i = 0; i < numLights; i++)
		{
			intensities[i] = light[i].intensities;
			position[i] = light[i].position;
			attenuation[i] = light[i].attenuation;
			ambientCoefficient[i] = light[i].ambientCoefficient;
		}
		shaders->setUniform("light.position", position, 4);
		shaders->setUniform("light.intensities", intensities, 4);
		shaders->setUniform("light.attenuation", attenuation, 4);
		shaders->setUniform("light.ambientCoefficient", ambientCoefficient, 4);

		shaders->setUniform("cameraPosition", camera.position());
		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asset.texture->object());

		if (asset.bumpMap != NULL)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, asset.bumpMap->object());
			shaders->setUniform("bumpTex", 1);
		}
		//bind VAO and draw
		glBindVertexArray(asset.vao);
		glDrawArrays(asset.drawType, asset.drawStart, asset.drawCount);

		//unbind everything
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaders->stopUsing();
	}
	virtual const char *GetName() { return "ModelInstance"; }
};