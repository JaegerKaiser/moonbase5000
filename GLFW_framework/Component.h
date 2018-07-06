#pragma once
#include "Camera.h"
#include "Light.h"
class Object;


class Component
{
protected:
	Object *pParent;

public:
	Component() : pParent(nullptr) {}
	void SetParent(Object *newParent) { pParent = newParent;  }
	virtual void Update(float deltaT) {}
	virtual void Render(const tdogl::Camera &camera, const Light *light, int numLights) const {}
	virtual const char* GetName() = 0;
};