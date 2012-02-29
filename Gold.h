#pragma once
#include "Object.h"

class Gold : public Object
{
public:
	Gold(int x, int y, int amount = 1);
	~Gold();

	bool handleCollision(Object* collider, MTV* mtv);
	int getAmount();
private:
	int mAmount;
};