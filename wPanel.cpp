#include "wPanel.h"

wPanel::wPanel(float x, float y, int width, int height, wId id, string textureSource)	: wComponent(x, y, width, height, id)
{
	// :TODO: Color key
	setTexture(textureSource);
}

wPanel::~wPanel()
{

}