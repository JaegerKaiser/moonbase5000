#pragma once
#include "GeomUtils.h"
#include "CollisionData.h"

class Collision
{
public:
	static bool SphereSphereCollision(const vec3 &r0, float rad0, const vec3 &r1, float rad1);
	static bool SphereCapsuleCollision(const vec3 &rSph, float radSph, const vec3 &r0Cap, const vec3 &r1Cap, float capRad);
	static bool SphereSphereCollision(const vec3 &r0, float rad0, const vec3 &r1, float rad1, vec3 &normal, float &depth);
	static bool SphereCapsuleCollision(const vec3 &rSph, float radSph, const vec3 &r0Cap, const vec3 &r1Cap, float capRad, vec3 &normal, float &depth);
	static bool SpherePlaneCollision(const vec3 &rSph, float radSph, const vec3 &rPlane, const vec3 &nPlane, vec3 &collNormal, float &collDepth);
	static bool SphereCubeCollision(const vec3 &cubeMin, const vec3 &cubeMax, const vec3 &centreSphere, float radSph, vec3 &collNormals, float &depth);
};