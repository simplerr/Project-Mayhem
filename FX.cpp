#include "FX.h"
#include "Graphics.h"
#include "Animation.h"


FX::FX(float x, float y, float w, float h, float life) : Object(x,y,w,h,SFX) 
{
	mLife = life;
}

FX::~FX()
{

}

void FX::draw()
{
	gGraphics->drawTexturedPolygon(getPolygon(), getTexture(), &mAnimation->getSourceRect());
}

void FX::update(float dt)
{
	mAnimation->animate(dt);
	if(mCounter > mLife)
		delete this;
	mCounter+=dt;
}