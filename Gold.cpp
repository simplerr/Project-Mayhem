#include "Gold.h"
#include "Player.h"
#include "Graphics.h"

Gold::Gold(int x, int y, int amount) : Loot("Gold", x, y)
{
	mAmount = amount;
	setDetailedCollision(false);
}
	
Gold::~Gold()
{

}

void Gold::draw()
{ 
	gGraphics->drawTexture(getTexture(), getPos().x, getPos().y, 16, 16);
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