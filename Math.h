#ifndef MATH_H
#define MATH_H



class Vector;
class Rect;

const float EPSILON = 0.00001f;                         ///< floating point EPSILON for single precision. todo: verify EPSILON value and usage
const float EPSILONSquared = EPSILON * EPSILON;         ///< EPSILON value squared
const float PI = 3.1415926f;                            ///< pi stored at a reasonable precision for single precision floating point.

class Math
{
public:
	Math();
	~Math();

	static float calculateAngle(Vector origin, Vector point);
	static float delta(float a, float b);
	static float distance(Vector v1, Vector v2);
	static bool pointInsideRect(Vector point, Rect rect);
	static int random(int min, int max);
private:
};

extern Math* gMath;

/// test for floating point equality within [-epsilon,+epsilon]

inline bool equal(float a, float b)
{
	const float d = a - b;
	if (d<EPSILON && d>-EPSILON) 
		return true;
	else 
		return false;
}

#endif