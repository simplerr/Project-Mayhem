#pragma once

#include "Object.h"

class Region : public Object
{
public:
	Region(float x, float y, int width, int height);
	~Region();
	void draw();
	void update();

private:
};

