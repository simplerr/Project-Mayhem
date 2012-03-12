#include "wLabel.h"
#include "WindowHandler.h"
#include "Graphics.h"

wLabel::wLabel(float x, float y, int width, int height, wId id, string label) :
	wComponent(x, y, width, height, id)
{
	mLabel = label;
}
	
wLabel::~wLabel()
{

}

void wLabel::draw()
{
	gGraphics->drawText(mLabel, getPos().x, getPos().y, SMALL_DX);
}
	