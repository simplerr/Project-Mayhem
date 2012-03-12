#include "HealthPotion.h"
#include "Player.h"

HealthPotion::HealthPotion(int x, int y, int hp) : Loot("Health Potion", x, y)
{
	mHp = hp;
	setDetailedCollision(false);
}
	
HealthPotion::~HealthPotion()
{

}

int HealthPotion::getHp()
{
	return mHp;
}

bool HealthPotion::handleCollision(Object* collider, MTV* mtv)
{
	if(collider->getType() == PLAYER) {
		Player* player = dynamic_cast<Player*>(collider);
		player->setHealth(player->getHealth() + mHp);
		setAlive(false);
	}

	return true;
}