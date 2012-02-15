#include "Structure.h"
#include "Graphics.h"

Structure::Structure(float x, float y, int width, int height, std::string textureSource)
	: Object(x, y, width, height, STRUCTURE, textureSource)
{
	setLayer(MIDDLE);
}

Structure::~Structure()
{
	
}

void Structure::draw() {
	gGraphics->drawTexturedPolygon(getPolygon(), getTexture());
}