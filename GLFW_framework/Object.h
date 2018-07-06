#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include "Light.h"
#include "CollisionData.h"

#include <list>

class Object
{
	glm::mat4 transform;
	std::list<Component*> components;
	CollisionData *pCollision;

public:
	Object(const glm::mat4 &transform) : transform(transform), pCollision(nullptr) {}
	~Object() 
	{
		for (auto c : components)
			delete c;
		if (pCollision != nullptr)
			delete pCollision;
	}
	void AddCollision(CollisionData *newCollision) { pCollision = newCollision; }

	CollisionData *GetCollision() const { return pCollision;  }

	void AddComponent(Component *pComponent) 
	{
		pComponent->SetParent(this);
		components.push_back(pComponent); 
	}
	void Update(float deltaT) 
	{
		for (auto pc : components)
			pc->Update(deltaT);
	}
	void Render(const tdogl::Camera &camera, const Light *light, int numLights) const
	{
		for (auto pc : components)
			pc->Render( camera, light, 4);
	}
	const glm::mat4& GetTransform() const { return transform; }
	const glm::vec3& GetPosition() const { return (vec3)transform[3]; }
	void SetTransform(const glm::mat4 &newTrans) { transform = newTrans; }
	Component *GetComponentByName(const char *name)
	{
		Component *returnValue = nullptr;
		for (auto pc : components)
		{
			if (strcmp(name, pc->GetName()) == 0)
			{
				returnValue = pc;
				break;
			}
		}
		return returnValue;
	}
};