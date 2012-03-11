#include "EnergyPotion.h"
#include "Player.h"

EnergyPotion::EnergyPotion(int x, int y, int energy) : Object(x, y, 32, 32, ENERGY_POTION, "Data\\imgs\\energy_potion.png")
{
	mEnergy = energy;
	setDetailedCollision(false);
}
	
EnergyPotion::~EnergyPotion()
{

}

int EnergyPotion::getEnergy()
{
	return mEnergy;
}

bool EnergyPotion::handleCollision(Object* collider, MTV* mtv)
{
	if(collider->getType() == PLAYER) {
		Player* player = dynamic_cast<Player*>(collider);
		player->setEnergy(player->getEnergy() + mEnergy);
		setAlive(false);
	}

	return true;
}