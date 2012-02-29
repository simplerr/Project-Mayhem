#pragma once

#include "Skill.h"

class Blink : public Skill
{
public:
	Blink();
	~Blink(){};

	void performSkill(Player* player);
private:
	float blinkRange[5];
};