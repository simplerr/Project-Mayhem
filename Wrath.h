#pragma once

#include "Skill.h"

class Wrath : public Skill
{
public:
	Wrath();
	~Wrath(){};

	void performSkill(Player* player);
private:
	float wrathDamage[5];
};