#include "Sniper.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Sound.h"

Sniper::Sniper(Player* player, Level* level) : Weapon(player, level)
{
	setOffset(Vector(0, 13));
	ProjectileData pd = gScrap->basicProjectile;
	pd.texturePath = "Data\\imgs\\sniper_bullet.png";
	pd.width = 20;
	pd.height = 8;
	pd.speed = 30;
	pd.damage = 3;
	setProjectileData(pd);
	setCooldown(0.8f);
	setTexture("Data/imgs/items/sniper_top.png");
	setWidth(50);
}

Sniper::~Sniper()
{

}

void Sniper::fire(Vector playerPos, float rotation)
{
	gAudio->playEffect("Data\\sounds\\fx\\sniper.wav");
	// Mouse
	Vector mousePos = gInput->mousePosition();
	Vector offset(8, 15);
	Vector pos = Vector(playerPos.x + cosf(rotation)*offset.x - sinf(rotation)*offset.y, 
		playerPos.y + sinf(rotation)*offset.x + cosf(rotation)*offset.y);

	getLevel()->addProjectile(getPlayer(), pos, gInput->mousePosition(), getProjectileData());

	setCounter(0.0f);
}