#pragma once
#include "glm/glm.hpp"
#include "Component.h"

using namespace glm;

class Projectile : public Component
{
	vec3 pos;
	vec3 vel;
	vec3 acc;
public:
	Projectile(const vec3 &pos, const vec3 &vel, const vec3 &acc)
		: pos(pos), vel(vel), acc(acc) {}
	virtual void Update(float deltaT)
	{
		pos += vel * deltaT;
		vel += acc * deltaT;
		// friction
		if (dot(vel,vel) > 0.0f)
		{
			vec3 fric = normalize(vel)*2.0f*deltaT;
			if (dot(fric, fric) > dot(vel, vel))
				vel = vec3(0, 0, 0);
			else
				vel -= fric;
		}
		// update transform of parent
		pParent->SetTransform(glm::translate(glm::mat4(), pos));
	}
	vec3 GetVelocity() { return vel; }
	void SetVelocity(const vec3 &newVel) { vel = newVel; }
	vec3 GetPosition() { return pos; }
	virtual const char* GetName() { return "Projectile"; }
};
