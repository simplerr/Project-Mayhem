#include "Skill.h"
#include "Player.h"
#include "Graphics.h"

Skill::Skill() : SlotItem()
{
	mTexture = gGraphics->loadTexture("Data\\imgs\\blink_icon.png");
}

Skill::~Skill() 
{
	ReleaseCOM(mTexture);
}

void Skill::performSkill(Player* player)
{
	// Temp message!!
	MessageBox(0, "asd", 0, 0);
}

void Skill::setTexture(string texture)
{
	mTexture = gGraphics->loadTexture(texture);
}

IDirect3DTexture9* Skill::getTexture()
{
	return mTexture;
}

SlotId Skill::getSlotId()
{
	return SKILL;
}