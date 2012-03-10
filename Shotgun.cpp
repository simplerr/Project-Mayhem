#include "Shotgun.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"

Shotgun::Shotgun(Player* player, Level* level) : Weapon(player, level)
{
	ProjectileData pd;
	pd.spread = 20;
	pd.texturePath = "Data\\imgs\\yellow_tile.bmp";
	setProjectileData(pd);
	setTexture("Data\\imgs\\vapen2.png");
}

Shotgun::~Shotgun()
{

}

void Shotgun::fire(Vector playerPos, float rotation)
{
	// Mouse
	Vector mousePos = gInput->mousePosition();
	Vector offset(55, 15);
	Vector pos = Vector(playerPos.x + cosf(rotation)*offset.x - sinf(rotation)*offset.y, 
		playerPos.y + sinf(rotation)*offset.x + cosf(rotation)*offset.y);

	getLevel()->addProjectile(getPlayer(), pos, gInput->mousePosition(), getProjectileData());

	setCounter(0.0f);
}