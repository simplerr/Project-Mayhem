#include "Region.h"
#include "Graphics.h"
#include "Level.h"

Region::Region(float x, float y, int width, int height)
	: Object(x+(width/2), y+(height/2), width, height, REGION, "Data\\imgs\\standard_box.bmp")
{
	setLayer(TOP);
	setCollidable(false);
}

Region::~Region()
{
	
}

void Region::draw() {
	if(getLevel()->isInEditor()) {
		gGraphics->drawRect(getRect(), D3DCOLOR_ARGB(100, 20,150,200));
	}
}

void Region::update() 
{
	
}