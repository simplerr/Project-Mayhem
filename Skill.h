#pragma once
#include <string>
#include "d3dUtil.h"
#include "SlotItem.h"

using namespace std;
class Player;

//! Base class for all skills
//! Do your magic Viktor!!
class Skill : public SlotItem
{
public:
	Skill();
	~Skill();

	void performSkill(Player* player);

	void setTexture(string texture);
	IDirect3DTexture9* getTexture();

	SlotId getSlotId();
private:
	IDirect3DTexture9* mTexture;
};