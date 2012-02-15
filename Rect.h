#ifndef RECT_H
#define RECT_H

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
	}

	Rect(float x, float y, int width, int height, int lol)	{
		left = x - width/2;
		right = x + width/2;
		top = y - height/2;
		bottom = y + height/2;
	}
		
	float getWidth()	{
		return right - left;
	}

	float getHeight()	{
		return bottom - top;
	}

	float left;
	float right;
	float top;
	float bottom;
};

#endif