#pragma once
#include <string>
#include "d3dUtil.h"
#include "SlotItem.h"
#include <string>

using namespace std;
class Player;

//! Base class for all skills
//! Do your magic Viktor!!
class Skill : public SlotItem
{
public:
	Skill();
	~Skill();

	void init();

	virtual void performSkill(Player* player);
	void doCooldown(float dt);

	void setIconTexture(string texture);
	void setEnergyCost(int cost)	{energyCost = cost;}
	void setPassive(bool b)			{passive = b;}

	int getLevel()					{return skillLevel;}
	int getEnergyCost()				{return energyCost;}
	bool isPassive()				{return passive;}

	bool canCast(Player* player);

	IDirect3DTexture9* getTexture();
	SlotId getSlotId();
private:
	int skillLevel;
	bool passive;
	int energyCost;
	IDirect3DTexture9* mTexture;
	string description;
	string name;
};