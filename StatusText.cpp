#include "Vector.h"
#include "Graphics.h"
#include "StatusText.h"

//! Constructor.
/**
@param text The text to draw.
@param time How long to draw the text (seconds).
*/
StatusText::StatusText(string source, float x, float y, int width, int height, float time)
{
	mTexture = gGraphics->loadTexture(source);
	mPos = Vector(x, y);
	mWidth = width;
	mHeight = height;
	mTime = time;
	mVisible = false;
}

//! Destructor.
StatusText::~StatusText()
{

}

//! Draws the status text.
void StatusText::draw()
{
	if(mTime > 0 && mVisible)	{
		gGraphics->drawTexture(mTexture, mPos.x, mPos.y, mWidth, mHeight);
	}
}

//! Updates the timer.
void StatusText::update(float dt)
{
	mTime -= dt;
}

//! Set the text and how long to draw it.
void StatusText::show(float time)
{
	mTime = time;
	mVisible = true;
}