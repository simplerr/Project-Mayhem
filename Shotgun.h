#pragma once

#include "Weapon.h"


class Shotgun : public Weapon
{
public:
	Shotgun(Player* player, Level* level);
	~Shotgun();

	void fire(Vector playerPos, float rotation);
};