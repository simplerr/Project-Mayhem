#pragma once
#include "Loot.h"

class HealthPotion : public Loot
{
public:
	HealthPotion(int x, int y, int hp = 10);
	~HealthPotion();

	bool handleCollision(Object* collider, MTV* mtv);
	int getHp();
private:
	int mHp;
};