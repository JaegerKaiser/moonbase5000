#pragma once
#include "glm/glm.hpp"

using namespace glm;

typedef enum
{
	CTYPE_NONE,
	CTYPE_SPHERE,
	CTYPE_CAPSULE,
	CTYPE_PLANE,
	CTYPE_CUBE,
	CTYPE_SIZE
} COLL_TYPE;

class CollisionData
{
protected:
	COLL_TYPE type;
public:
	CollisionData() : type(CTYPE_NONE) {}
	CollisionData(COLL_TYPE type) : type(type) {}
	COLL_TYPE GetType() const { return type; }
};

class SphereColl : public CollisionData
{
	vec3 r;
	float radius;
public:
	SphereColl(const vec3 &r, float radius) : CollisionData(CTYPE_SPHERE), r(r), radius(radius) {};
	const vec3 &GetPos() const { return r; };
	vec3 GetPos(const mat4 &trans) { return (vec3)(trans*vec4(r, 1.0f)); }
	float GetRadius() const { return radius; }
};

class CapsuleColl : public CollisionData
{
	vec3 r0, r1;
	float rad;
public:
	CapsuleColl(const vec3 &r0, const vec3 &r1, float rad) : CollisionData(CTYPE_CAPSULE), r0(r0), r1(r1), rad(rad) {};
	const vec3 &GetR0() const { return r0; };
	const vec3 &GetR1() const { return r1; };
	vec3 GetR0(const mat4 &trans) const { return (vec3)(trans*vec4(r0,1.0f)); };
	vec3 GetR1(const mat4 &trans) const { return (vec3)(trans*vec4(r1,1.0f)); };
	float GetRadius() const { return rad; }
};

class PlaneColl : public CollisionData
{
	// infinite plane
	vec3 r, n;
public:
	PlaneColl(const vec3 &r, const vec3 &n) : CollisionData(CTYPE_PLANE), r(r), n(n) {};
	vec3 GetPos(const mat4 &trans) { return (vec3)(trans*vec4(r, 1.0f)); };
	vec3 GetNormal(const mat4 &trans) 
	{
		mat4 rot = trans;
		// zero the position part
		rot[3] = vec4();
		return (vec3)(rot*vec4(n, 1.0f));
	};
};

class CubeColl : public CollisionData
{
	vec3 centre;
	float length, width, height;
public:
	CubeColl(const vec3 centre, float length, float width, float height) : CollisionData(CTYPE_CUBE), centre(centre), length(length), width(width), height(height) {};
	const vec3 &getCentre() const { return centre; }
	vec3 getCentre(const mat4 &trans) const { return (vec3)(trans*vec4(centre, 1.0f)); }
	float getLength() const { return length; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }
};