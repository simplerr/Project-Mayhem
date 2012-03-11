#pragma once

#include "Skill.h"

class SolarBolts : public Skill
{
public:
	SolarBolts();
	~SolarBolts(){};

	void performSkill(Player* player);
private:
	float spiritDamage[5];
};