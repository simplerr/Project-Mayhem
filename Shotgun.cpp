#include "Shotgun.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Sound.h"

Shotgun::Shotgun(Player* player, Level* level) : Weapon(player, level)
{
	ProjectileData pd;
	pd.spread = 20;
	pd.texturePath = "Data\\imgs\\yellow_tile.bmp";
	pd.speed = 17.0f;
	setProjectileData(pd);
	setTexture("Data\\imgs\\items\\shotgun_top.png");
	setOffset(Vector(0, 12));
}

Shotgun::~Shotgun()
{

}

void Shotgun::fire(Vector playerPos, float rotation)
{
	gAudio->playEffect("Data\\sounds\\fx\\Rifle.wav");
	// Mouse
	Vector mousePos = gInput->mousePosition();
	Vector offset(15, 15);

	for(int i = 0; i < 5; i++) {
		Vector pos = Vector(playerPos.x + cosf(rotation)*offset.x - sinf(rotation)*offset.y, 
		playerPos.y + sinf(rotation)*offset.x + cosf(rotation)*offset.y);

		getLevel()->addProjectile(getPlayer(), pos, gInput->mousePosition(), getProjectileData());
	}
	
	setCounter(0.0f);
}