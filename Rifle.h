#pragma once

#include "Weapon.h"

class Rifle : public Weapon
{
public:
	Rifle(Player* player, Level* level);
	~Rifle();

	void fire(Vector playerPos, float rotation);
};