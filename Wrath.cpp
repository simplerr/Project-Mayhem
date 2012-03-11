#include "Wrath.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Projectile.h"

Wrath::Wrath() {
	//Skill Data:
	wrathDamage[0] = 5;
	wrathDamage[1] = 10;
	wrathDamage[2] = 15;
	wrathDamage[3] = 20;
	wrathDamage[4] = 25;
	
	init();
	setIconTexture("Data\\imgs\\fire_bolt.png");
	setName("Roar of Wrath");
	setDescription("Release the wrath of old memories upon your foes, ripping them to shreds.");

	setEnergyCost(5);
}

void Wrath::performSkill(Player* player) 
{
	if(canCast(player)) {
		ProjectileData pd;
		pd.texturePath = "Data\\imgs\\fire_projectile.png";
		pd.damage = wrathDamage[getLevel()];
		pd.spread = 1;
		pd.width = 40;
		pd.height = 20;
		pd.speed = 10;
		pd.range = 400;
		Vector pos = gInput->mousePosition();
		player->getLevel()->addProjectile(player, player->getPos(), pos, pd);

		player->costEnergy(getEnergyCost());
	}
}