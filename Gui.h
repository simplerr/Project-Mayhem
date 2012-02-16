#pragma once
#include "d3dUtil.h"
#include "Container.h"
#include <vector>
using namespace std;

class Player;
class Skill;

class Gui : public Container
{
public:
	Gui(Player* player);
	~Gui();

	void update(float dt);
	void draw();
	void addSkill(Skill* skill);
private:
	IDirect3DTexture9*	mSlotTexture;
	Player*				mPlayer;
};