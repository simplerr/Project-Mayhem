#pragma once
#include "d3dUtil.h"

class Player;

class Gui
{
public:
	Gui(Player* player);
	~Gui();

	void update(float dt);
	void draw();
private:
	IDirect3DTexture9*	mSlotTexture;
	Player*				mPlayer;
	Vector				mPos;
	int					mWidth, mHeight;
};