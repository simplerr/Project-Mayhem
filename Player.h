#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Animation.h"

class Level;
class Inventory;
class Gui;

class Player : public Object 
{
public:
	Player(float x, float y);
	~Player();

	void update(float dt);
	void draw();

	void handleInput();
	void attack(/*KEY/WPN used*/);
	void move(float dx, float dy);

	void setCooldown(float cooldown);

	float getHealth();
	float getMaxHealth();
	float getEnergy();
	float getMaxEnergy();

private:
	Animation*	mAnimation;
	Inventory*	mInventory;
	Gui*		mGui;
	float mHealth;
	float mEnergy;
	float mMaxHealth;
	float mMaxEnergy;
	float mMoveSpeed;
	float mAngle;
	float mCooldown;
	float mCounter;
};

#endif