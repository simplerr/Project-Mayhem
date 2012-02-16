#include "Scrap.h"
#include "Object.h"
#include "Rect.h"

Scrap::Scrap() 
{
	tileObject = new Object(0, 0, 40, 40, STRUCTURE, "Data\\imgs\\standard_box.bmp");
	rect = new Rect(10,10,40,40,0);
}

Scrap::~Scrap() 
{
	delete tileObject;
	delete rect;
}