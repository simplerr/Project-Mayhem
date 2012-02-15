#include "Math.h"
#include "Vector.h"
#include "Rect.h"
#include <time.h>
#include <stdlib.h>


Math::Math()
{
	srand(time(NULL));
}

Math::~Math()
{

}

float Math::calculateAngle(Vector origin, Vector point)
{
	point = point - origin;
	return atan2f(point.y, point.x);
}


float Math::delta(float a, float b) 
{
	return abs(a - b);
}
float Math::distance(Vector v1, Vector v2) 
{
	return sqrt(pow(delta(v1.x, v2.x), 2)+pow(delta(v1.y, v2.y), 2));
}

int Math::random(int min, int max)
{
	return rand() % (max - min) + min;
}

bool Math::pointInsideRect(Vector point, Rect rect)
{
	if(point.x > rect.left && point.x < rect.right && point.y > rect.top && point.y < rect.bottom)
		return true;
	else
		return false;
}