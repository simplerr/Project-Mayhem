#include "SolarBolts.h"
#include "Skill.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Projectile.h"

SolarBolts::SolarBolts() {
	//Skill Data:
	spiritDamage[0] = 3;
	spiritDamage[1] = 10;
	spiritDamage[2] = 15;
	spiritDamage[3] = 20;
	spiritDamage[4] = 25;
	
	setIconTexture("Data\\imgs\\orb.png");
	setName("Lunar Spirits");
	setDescription("Years of studying the moonlight through the narrow cell window has yielded the ability to call upon several Lunar Spirits, each dealing moderate damage.");
	setEnergyCost(5);
}

void SolarBolts::performSkill(Player* player) 
{
	if(canCast(player)) {
		ProjectileData pd;
		pd.texturePath = "Data\\imgs\\orb.png";
		pd.damage = spiritDamage[getLevel()];
		pd.spread = 1;
		pd.width = 21;
		pd.height = 20;
		pd.speed = 6;
		pd.range = 400;
		Vector pos = gInput->mousePosition();
		for(float f = -0.5f; f < 0.5f; f+=0.1f) {
			player->getLevel()->addProjectile(player, player->getPos(), pos, pd);
			Projectile* p = dynamic_cast<Projectile*>(player->getLevel()->getObjectList()->back());
			p->rotate(f);
		}

		player->costEnergy(getEnergyCost());
	}
}