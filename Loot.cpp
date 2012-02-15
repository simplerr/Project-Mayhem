#include "Loot.h"
#include "ItemHandler.h"

Loot::Loot(string name, int x, int y) : Object(x, y, 32, 32, LOOT, gItemHandler->getData(name).textureSource)
{
	mName = name;
}
	
Loot::~Loot()
{

}

string Loot::getName()
{
	return mName;
}