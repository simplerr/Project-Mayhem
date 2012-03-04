#include "Weapon.h"
#include "Graphics.h"
#include "Input.h"
#include "Level.h"
#include "Player.h"

Weapon::Weapon(Player* player, Level* level)
{
	mPlayer = player;
	mLevel = level;
	mCounter = 0.0f;
	mTexture = 	gGraphics->loadTexture("Data\\imgs\\vapen1.png");
	mOffset = Vector(30, 16);
	setCooldown(0.55f);
} 
	
Weapon::~Weapon()
{
	ReleaseCOM(mTexture);
}

void Weapon::update(float dt)
{
	mCounter += dt;
}

void Weapon::draw(Vector playerPos, float rotation)
{
	gGraphics->drawTexture(mTexture, playerPos.x + cosf(rotation)*mOffset.x - sinf(rotation)*mOffset.y, 
		playerPos.y + sinf(rotation)*mOffset.x + cosf(rotation)*mOffset.y, 64, 16, rotation);
}

void Weapon::fire(Vector playerPos, float rotation)
{
	// Mouse
	Vector mousePos = gInput->mousePosition();
	Vector pos = Vector(playerPos.x + cosf(rotation)*mOffset.x - sinf(rotation)*mOffset.y, 
		playerPos.y + sinf(rotation)*mOffset.x + cosf(rotation)*mOffset.y);

	mLevel->addProjectile(mPlayer, gInput->mousePosition());

	mCounter = 0.0f;
}

void Weapon::setOffset(Vector offset)
{
	mOffset = offset;
}

bool Weapon::isReady()
{
	if(mCounter >= mCooldown)
		return true;
	else
		return false;
}

void Weapon::setCooldown(float cooldown)
{
	mCooldown = cooldown;
}
