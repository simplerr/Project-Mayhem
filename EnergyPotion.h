#pragma once
#include "Object.h"

class EnergyPotion : public Object
{
public:
	EnergyPotion(int x, int y, int energy = 10);
	~EnergyPotion();

	bool handleCollision(Object* collider, MTV* mtv);
	int getEnergy();
private:
	int mEnergy;
};