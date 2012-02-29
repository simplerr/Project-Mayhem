#ifndef RECT_H
#define RECT_H

#include "Vector.h"

// The standard class RECT doesn't support decimals
class Rect
{
public:
	Rect() {};
	Rect(float _left, float _right, float _top, float _bottom)	{
		left = _left;
		right = _right;
		top = _top;
		bottom = _bottom;
		width = gMath->delta(left, right);
		height = gMath->delta(top, bottom);
	}

	Rect(float x, float y, int _width, int _height, int lol)	{
		left = x - _width/2;
		right = x + _width/2;
		top = y - _height/2;
		bottom = y + _height/2;
		width = _width;
		height = _height;
	}
		
	float getWidth()	{
		return right - left;
	}

	float getHeight()	{
		return bottom - top;
	}

	void setPos(Vector v) 
	{
		left = v.x - width/2;
		right = v.x + width/2;
		top = v.y - height/2;
		bottom = v.y + height/2;
	}

	void setSize(int _width, int _height)
	{
		float x = left + (width/2);
		float y = top + (height/2);
		left = x - _width/2;
		right = x + _width/2;
		top = y - _height/2;
		bottom = y + _height/2;
		width = _width;
		height = _height;
	}
	void setRect(float l, float r, float t, float b)
	{
		left = l;
		right = r;
		top = t;
		bottom = b;
		width = gMath->delta(left, right);
		height = gMath->delta(top, bottom);
	}

	float left;
	float right;
	float top;
	float bottom;
private:
	float width;
	float height;
};

#endif