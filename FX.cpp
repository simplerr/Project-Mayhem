#include "FX.h"
#include "Graphics.h"
#include "Animation.h"


FX::FX(float x, float y, float w, float h, float life, int frames, std::string textureSource) : Object(x,y,w,h,SFX, textureSource) 
{
	mLife = life;
	mCounter = 0;
	mAnimation = new Animation(w,h,life/frames, frames, frames);
	setLayer(TOP);
	setCollidable(false);
	
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
		this->setAlive(false);
	mCounter+=dt;
}