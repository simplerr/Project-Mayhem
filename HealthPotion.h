#pragma once
#include "Object.h"

class HealthPotion : public Object
{
public:
	HealthPotion(int x, int y, int hp = 10);
	~HealthPotion();

	bool handleCollision(Object* collider, MTV* mtv);
	int getHp();
private:
	int mHp;
};