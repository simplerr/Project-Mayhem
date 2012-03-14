#include "Rifle.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Sound.h"

Rifle::Rifle(Player* player, Level* level) : Weapon(player, level)
{
	setOffset(Vector(0, 13));
	setCooldown(0.1f);
	setTexture("Data\\imgs\\items\\rifle_top.png");
}

Rifle::~Rifle()
{

}

void Rifle::fire(Vector playerPos, float rotation)
{
	gAudio->playEffect("Data\\sounds\\fx\\Gun.wav");
	// Mouse
	Vector mousePos = gInput->mousePosition();
	Vector offset(8, 15);
	Vector pos = Vector(playerPos.x + cosf(rotation)*offset.x - sinf(rotation)*offset.y, 
		playerPos.y + sinf(rotation)*offset.x + cosf(rotation)*offset.y);

	getLevel()->addProjectile(getPlayer(), pos, gInput->mousePosition(), getProjectileData());

	setCounter(0.0f);
}