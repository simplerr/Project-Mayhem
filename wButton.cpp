#include "wButton.h"
#include "Graphics.h"

wButton::wButton(int x, int y, int width, int height, string label, wId id, string textureSource)
	:wComponent(x, y, width, height, id)
{
	// Set label and load texture
	mLabel = label;

	// :NOTE: Why does it have an own texture? It can use wComponents instead!
	mTexture = gGraphics->loadTexture(textureSource);
	mBkgdTexture = gGraphics->loadTexture("Data\\imgs\\bag_slot.png");
}

wButton::~wButton()
{
	// Release the texture
	ReleaseCOM(mTexture);
	ReleaseCOM(mBkgdTexture);
}
	
void wButton::draw()
{
	// Draw the texture and labelR
	gGraphics->drawTexture(mBkgdTexture, getPos().x, getPos().y, getWidth()+5, getHeight()+5);
	gGraphics->drawTexture(mTexture, getPos().x, getPos().y, getWidth(), getHeight());
	//gGraphics->drawText((char*)mLabel.c_str(), getPos().x - getWidth()/2 + 4, getPos().y - 10, TextType::SMALL_DX);
}
	
void wButton::onPress()
{
	// Send the ID and the Label
	callback(getId(), wMessage(mLabel));
}

void wButton::onHoover()
{
	// Change the texture
}