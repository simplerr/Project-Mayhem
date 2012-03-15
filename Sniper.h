#pragma once

#include "Weapon.h"


class Sniper : public Weapon
{
public:
	Sniper(Player* player, Level* level);
	~Sniper();

	void fire(Vector playerPos, float rotation);
};