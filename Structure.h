#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Object.h"

class Structure : public Object
{
public:
	Structure(float x, float y, int width, int height, std::string textureSource = "Data\\imgs\\standard_box.bmp");
	~Structure();
	void draw();

private:
};

#endif