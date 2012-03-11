#include "wComponent.h"
#include "Rect.h"
#include "Graphics.h"
#include <Windows.h>

wComponent::wComponent(int x, int y, int width, int height, wId id)
{
	setPos(Vector(x, y));
	setWidth(width);
	setHeight(height);
	setId(id);
	setActive(false);
	mTexture = NULL;
	setZ(0);
}
	
wComponent::~wComponent()
{
	
}

void wComponent::update(float dt)
{

}
	
void wComponent::draw()
{
	if(mTexture != NULL)
		gGraphics->drawTexture(mTexture, getPos().x, getPos().y, getWidth(), getHeight());
	else
		gGraphics->drawRect(getPos().x, getPos().y, getWidth(), getHeight());	// :TODO: ............
}

bool wComponent::onPress()
{
	return false;
}

void wComponent::onHoover()
{

}

void wComponent::onText()
{

}

Rect wComponent::getRect()
{
	Rect rect;
	// mPosition is in the center of the object
	rect.left = getPos().x - getWidth()/2;
	rect.right = getPos().x + getWidth()/2;
	rect.top = getPos().y - getHeight()/2;
	rect.bottom = getPos().y + getHeight()/2;

	return rect;
}

void wComponent::setTexture(string textureSource)
{
	mTexture = gGraphics->loadTexture(textureSource);
}

void wComponent::setActive(bool active)
{
	mActive = active;
}
	
bool wComponent::getActive()
{
	return mActive;
}