#include "Mine.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Projectile.h"

Mine::Mine() {
	//Skill Data:
	mineDamage[0] = 5;
	mineDamage[1] = 10;
	mineDamage[2] = 15;
	mineDamage[3] = 20;
	mineDamage[4] = 25;

	init();
	setName("Plant Mine");
	setDescription("Place a mine at your location. The mine's detector activates the device upon enemy detection, dealing heavy damage.");
	setIconTexture("Data\\imgs\\mine.png");
	setEnergyCost(5);
}

void Mine::performSkill(Player* player) 
{
	if(canCast(player)) {
		ProjectileData pd;
		pd.texturePath = "Data\\imgs\\mine.png";
		pd.damage = mineDamage[getLevel()];
		pd.spread = 1;
		pd.width = 30;
		pd.height = 30;
		pd.speed = 0;
		pd.range = 400;
		Vector pos = gInput->mousePosition();
		player->getLevel()->addProjectile(player, player->getPos(), pos, pd);

		player->costEnergy(getEnergyCost());
	}
}