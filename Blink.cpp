#include "Blink.h"
#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Vector.h"

Blink::Blink() {
	//Skill Data:
	blinkRange[0] = 100;
	blinkRange[1] = 150;
	blinkRange[2] = 200;
	blinkRange[3] = 300;
	blinkRange[4] = 400;
	
	setIconTexture("Data\\imgs\\blink_icon.png");
	init();
	setEnergyCost(5);
}

void Blink::performSkill(Player* player) {
	if(canCast(player)) {
		Vector pos = gInput->mousePosition();
		if(pos.x < 0 || pos.x > player->getLevel()->getWidth() || pos.y < 0  || pos.y > player->getLevel()->getHeight())
			return;

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
		player->costEnergy(getEnergyCost());
	}
}

