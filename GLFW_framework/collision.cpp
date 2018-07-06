#include "Collision.h"

bool Collision::SphereSphereCollision(const vec3 &r0, float rad0, const vec3 &r1, float rad1)
{
	return GeomUtils::Distance( r0, r1 ) < rad0+rad1;
}

bool Collision::SphereCapsuleCollision(const vec3 &rSph, float radSphere, const vec3 &r0cap, const vec3 &r1cap, float radcap )
{
	return GeomUtils::PointToFiniteLineDistance(r0cap, r1cap, rSph) < radcap + radSphere;
}

bool Collision::SphereSphereCollision(const vec3 &r0, float rad0, const vec3 &r1, float rad1, vec3 &norm, float &depth)
{
	depth = rad0 + rad1 - GeomUtils::Distance(r0, r1);
	if (depth > 0.0f)
	{
		norm = normalize(r1 - r0);
		return true;
	}
	return false;
}

bool Collision::SphereCapsuleCollision(const vec3 &rSph, float radSphere, const vec3 &r0cap, const vec3 &r1cap, float radcap, vec3 &norm, float &depth)
{
	vec3 nearPoint = GeomUtils::NearestPointOnFiniteLine(r0cap, r1cap, rSph);
	depth = radSphere + radcap - GeomUtils::Distance(rSph, nearPoint);
	if (depth > 0.0f)
	{
		norm = normalize(rSph - nearPoint);
		return true;
	}
	return false;
}

bool Collision::SpherePlaneCollision(const vec3 &rSph, float radSph, const vec3 &rPlane, const vec3 &nPlane, vec3 &collNormal, float &depth)
{
	float distance = fabs(dot(rSph - rPlane, nPlane));
	depth = radSph - distance;
	if (depth > 0.0f)
	{
		collNormal = nPlane;
		return true;
	}
	return false;
}

// THIS TOOK ME SO LONG TO DO BUT I'M SO HAPPY I FIGURED IT OUT, IT WAS SUCH A STUPID ERROR, THE Z-AXIS NORMAL WAS POINTING INSIDE THE CUBE RATHER THAN OUT
bool Collision::SphereCubeCollision(const vec3 &cubeMin, const vec3 &cubeMax, const vec3 &centreSphere, float radSph, vec3 &collNormal, float &depth)
{
	float radSquared = radSph;
	vec3 normal;

	if (centreSphere.x < cubeMin.x)
	{
		radSquared -= fabs(centreSphere.x - cubeMin.x);
		depth -= radSph - centreSphere.x - cubeMax.x;
		
		vec3 a, b, c, ab, bc;

		a = cubeMin; b = vec3(cubeMin.x, cubeMax.y, cubeMin.z); c = vec3(cubeMin.x, cubeMin.y, cubeMax.z);
		ab = b - a; bc = c - b;

		normal -= normalize(cross(ab, bc));
	}
	else if (centreSphere.x > cubeMax.x)
	{
		radSquared -= fabs(centreSphere.x - cubeMax.x);
		depth += radSph - centreSphere.x - cubeMax.x;

		vec3 a, b, c, ab, bc;

		a = cubeMax; b = vec3(cubeMax.x, cubeMin.y, cubeMax.z); c = vec3(cubeMax.x, cubeMin.y, cubeMin.z);
		ab = b - a; bc = c - b;

		normal += normalize(cross(ab, bc));
	}

	if (centreSphere.y < cubeMin.y)
	{
		radSquared -= fabs(centreSphere.y - cubeMin.y);
		depth -= radSph - centreSphere.y - cubeMax.y;

		vec3 a, b, c, ab, bc;

		a = cubeMin; b = vec3(cubeMin.x, cubeMin.y, cubeMax.z); c = vec3(cubeMax.x, cubeMin.y, cubeMin.z);
		ab = b - a; bc = c - b;

		normal -= normalize(cross(ab, bc));
	}
	else if (centreSphere.y > cubeMax.y)
	{
		radSquared -= fabs(centreSphere.y - cubeMax.y);
		depth += radSph - centreSphere.y - cubeMax.y;

		vec3 a, b, c, ab, bc;

		a = cubeMax; b = vec3(cubeMin.x, cubeMax.y, cubeMax.z); c = vec3(cubeMax.x, cubeMax.y, cubeMin.z);
		ab = b - a; bc = c - b;

		normal += normalize(cross(ab, bc));
	}

	if (centreSphere.z < cubeMin.z)
	{
		radSquared -= fabs(centreSphere.z - cubeMin.z);
		depth -= radSph - centreSphere.z - cubeMax.z;

		vec3 a, b, c, ab, bc;

		a = cubeMin; b = vec3(cubeMax.x, cubeMin.y, cubeMin.z); c = vec3(cubeMax.x, cubeMax.y, cubeMin.z);
		ab = b - a; bc = c - b;

		normal -= normalize(cross(ab, bc));
	}
	else if (centreSphere.z > cubeMax.z)
	{
		radSquared -= fabs(centreSphere.z - cubeMax.z);
		depth += radSph - centreSphere.z - cubeMax.z;

		vec3 a, b, c, ab, bc;

		a = cubeMax; b = vec3(cubeMin.x, cubeMin.y, cubeMax.z); c = vec3(cubeMax.x, cubeMin.y, cubeMax.z);
		ab = b - a; bc = c - b;

		normal += normalize(cross(ab, bc));
	}

	if (radSquared > 0)
	{
		depth = radSquared;
		collNormal = normal;
		return true;
	}
	return false;
}