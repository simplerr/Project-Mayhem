#pragma once

#include "Skill.h"

class Mine : public Skill
{
public:
	Mine();
	~Mine(){};

	void performSkill(Player* player);
private:
	float mineDamage[5];
	float mineDuration[5];
};