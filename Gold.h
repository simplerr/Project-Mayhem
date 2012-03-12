#pragma once
#include "Loot.h"

class Gold : public Loot
{
public:
	Gold(int x, int y, int amount = 1);
	~Gold();

	void draw();

	bool handleCollision(Object* collider, MTV* mtv);
	int getAmount();
private:
	int mAmount;
};