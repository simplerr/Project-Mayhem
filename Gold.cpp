#include "Gold.h"

Gold::Gold(int x, int y, int amount) : Object(x, y, 10, 10, GOLD_COIN, "Data\\imgs\\gold_coin.png")
{
	mAmount = amount;
}
	
Gold::~Gold()
{

}

int Gold::getAmount()
{
	return mAmount;
}