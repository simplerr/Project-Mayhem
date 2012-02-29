#include "Gold.h"
#include "Player.h"

Gold::Gold(int x, int y, int amount) : Object(x, y, 10, 10, GOLD_COIN, "Data\\imgs\\gold_coin.png")
{
	mAmount = amount;
	setDetailedCollision(false);
}
	
Gold::~Gold()
{

}

int Gold::getAmount()
{
	return mAmount;
}

bool Gold::handleCollision(Object* collider, MTV* mtv)
{
	if(collider->getType() == PLAYER) {
		dynamic_cast<Player*>(collider)->addGold(mAmount);
		setAlive(false);
	}

	return true;
}