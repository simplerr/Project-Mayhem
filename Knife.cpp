#include "Knife.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"

Knife::Knife(Player* player, Level* level) : Weapon(player, level)
{
	ProjectileData pd;
	pd.spread = 1;
	pd.range = 20;
	pd.speed = 0;
	pd.width = 50;
	pd.height = 8;
	pd.texturePath = "Data\\imgs\\kniv.png";
	pd.lifetime = 0.15f;
	pd.followPlayer = true;
	setProjectileData(pd);
	setVisible(false);
	setOffset(Vector(-20, 0));
	setCooldown(.4f);
}

Knife::~Knife()
{

}

void Knife::fire(Vector playerPos, float rotation)
{
	// Mouse
	Vector mousePos = gInput->mousePosition();
	Vector offset(35, 15);
	Vector pos = Vector(playerPos.x + cosf(rotation)*offset.x - sinf(rotation)*offset.y, 
		playerPos.y + sinf(rotation)*offset.x + cosf(rotation)*offset.y);

	getLevel()->addProjectile(getPlayer(), pos, gInput->mousePosition(), getProjectileData());

	setCounter(0.0f);
}