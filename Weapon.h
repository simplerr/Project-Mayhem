#pragma once
#include "d3dUtil.h"
#include "Vector.h"

class Level;
class Player;

class Weapon
{
public:
	Weapon(Player* player, Level* level);
	virtual	~Weapon();
	virtual void fire(Vector playerPos, float rotation);

	void update(float dt);
	void draw(Vector playerPos, float rotation);
	void setOffset(Vector offset);
	void setCooldown(float cooldown);
	bool isReady();
private:
	IDirect3DTexture9* mTexture;
	Vector mOffset;
	Player* mPlayer;
	Level* mLevel;
	float mCooldown;
	float mCounter;
};