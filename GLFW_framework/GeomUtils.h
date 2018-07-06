#pragma once
#include "glm/glm.hpp"

using namespace glm;
class GeomUtils
{
	public:

	static float Distance(const vec3 &p0, const vec3 &p1)
	{
		return length(p1 -  p0);
	}

	static vec3 NearestPointOnInfLine(const vec3 &rLine, const vec3 &nLine, const vec3 &point)
	{
		vec3 nNorm = normalize(nLine);
		float dotProd = dot( point - rLine, nNorm );
		return rLine + nNorm*dotProd;
	}

	static float PointToInfLineDistance(const vec3 &rLine, const vec3 &nLine, const vec3 &point)
	{
		return Distance( point, NearestPointOnInfLine( rLine, nLine, point ) );
	}

	static vec3 NearestPointOnFiniteLine(const vec3 &rLine0, const vec3 &rLine1, const vec3 &point)
	{
		vec3 line = rLine1 - rLine0;
		float sqMag = dot(line, line);

		if (sqMag == 0.0f)
			return rLine0;

		float dotProd = dot(point - rLine0, line);
		float l = dotProd / sqMag;

		if (l < 0.0f)
			return rLine0;
		else if (l > 1.0f)
			return rLine1;
		else
			return rLine0 + (rLine1 - rLine0) * l;
	}

	static float PointToFiniteLineDistance(const vec3 &rLine0, const vec3 &rLine1, const vec3 &point)
	{
		return Distance( point, NearestPointOnFiniteLine( rLine0, rLine1, point ) );
	}

};