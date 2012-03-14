#include "Weapon.h"
#include "Graphics.h"
#include "Input.h"
#include "Level.h"
#include "Player.h"
#include "Sound.h"

Weapon::Weapon(Player* player, Level* level)
{
	mPlayer = player;
	mLevel = level;
	mCounter = 0.0f;
	setTexture("Data/imgs/items/sniper_top.png");
	mOffset = Vector(30, 16);
	setCooldown(0.55f);
	setProjectileData(gScrap->basicProjectile);
	setVisible(true);
	setWidth(40);
	setHeight(10);
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
	if(isVisible()) {
		gGraphics->drawTexture(mTexture, playerPos.x + cosf(rotation)*mOffset.x - sinf(rotation)*mOffset.y, 
			playerPos.y + sinf(rotation)*mOffset.x + cosf(rotation)*mOffset.y, mWidth, mHeight, rotation);
	}
}

void Weapon::fire(Vector playerPos, float rotation)
{
	// Mouse
	gAudio->playEffect("Data\\sounds\\fx\\Rifle2.wav");
	Vector mousePos = gInput->mousePosition();
	Vector offset(55, 15);
	Vector pos = Vector(playerPos.x + cosf(rotation)*offset.x - sinf(rotation)*offset.y, 
		playerPos.y + sinf(rotation)*offset.x + cosf(rotation)*offset.y);

	mLevel->addProjectile(mPlayer, pos, gInput->mousePosition());

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

Level* Weapon::getLevel()
{
	return mLevel;
}
	
Player* Weapon::getPlayer()
{
	return mPlayer;
}

void Weapon::setCounter(float count)
{
	mCounter = count;
}

void Weapon::setProjectileData(ProjectileData projectileData)
{
	mProjectileData = projectileData;
}

ProjectileData Weapon::getProjectileData()
{
	return mProjectileData;
}

void Weapon::setTexture(string source)
{
	mTexture = gGraphics->loadTexture(source);
}

void Weapon::setVisible(bool visible)
{
	mVisible = visible;
}
	
bool Weapon::isVisible()
{
	return mVisible;
}

void Weapon::setWidth(int width)
{
	mWidth = width;
}
	
void Weapon::setHeight(int height)
{
	mHeight = height;
}