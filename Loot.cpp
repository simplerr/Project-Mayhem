#include "Loot.h"
#include "ItemHandler.h"

Loot::Loot(string name, int x, int y) : Object(x, y, 32, 32, LOOT, gItemHandler->getData(name).textureSource)
{
	mName = name;
	mCounter = 10.0f;
}
	
Loot::~Loot()
{

}

void Loot::update(float dt)
{
	mCounter -= dt;

	if(mCounter <= 0)
		setAlive(false);
}
	
void Loot::draw()
{
	Object::draw();
}

void Loot::setLifetime(float lifetime)
{
	mCounter = lifetime;
}

string Loot::getName()
{
	return mName;
}