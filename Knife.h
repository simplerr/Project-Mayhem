#pragma once

#include "Weapon.h"

class Knife : public Weapon
{
public:
	Knife(Player* player, Level* level);
	~Knife();

	void update(float dt);
	void fire(Vector playerPos, float rotation);
private:

};