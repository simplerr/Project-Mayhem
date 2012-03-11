#include "Blink.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Vector.h"
#include "FX.h"

Blink::Blink() {
	//Skill Data:
	blinkRange[0] = 100;
	blinkRange[1] = 150;
	blinkRange[2] = 200;
	blinkRange[3] = 300;
	blinkRange[4] = 400;
	
	init();
	setName("Blink");
	setDescription("Instantly move to or towards the targeted location.");
	setIconTexture("Data\\imgs\\blink_icon.png");
	setEnergyCost(5);
}

void Blink::performSkill(Player* player) {
	if(canCast(player)) {
		player->costEnergy(getEnergyCost());
		Vector pos = gInput->mousePosition();

		if(pos.x < 0 || pos.x > player->getLevel()->getWidth() || pos.y < 0  || pos.y > player->getLevel()->getHeight())
			return;
		player->getLevel()->addObject(new FX(player->getPos().x, player->getPos().y, 64,64,0.3f,4, "Data\\imgs\\blink_fx.png"));

		float angle = gMath->calculateAngle(player->getPos(), pos);
		if(pos.length() < blinkRange[getLevel()]) 
		{
			for(int i = 0; i<= pos.length(); i++)
				player->getLevel()->moveObjects(-cos(angle)*2, -sin(angle)*2);
		}
		else 
		{
			for(int i = 0; i<= blinkRange[getLevel()]; i+=2)
				player->getLevel()->moveObjects(-cos(angle)*2, -sin(angle)*2);
		}
	}
}

