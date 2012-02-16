#ifndef COLLISION_H
#define COLLISION_H
#include "cPolygon.h"
#include "Vector.h"

class MTV
{
public:
	MTV(){};
	MTV(double d, Vector axis)	{
		pushX = d*axis.x;
		pushY = d*axis.y;
		normal = axis;
	};

	void inverse()	{
		pushX *= -1;
		pushY *= -1;
		normal.x *= -1;
		normal.y *= -1;
	}

	bool   collision;
	double pushX;
	double pushY;
	Vector  normal;
};

MTV checkCollision (cPolygon poly1, cPolygon poly2);

#endif