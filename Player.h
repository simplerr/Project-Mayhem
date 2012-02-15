#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Animation.h"

class Level;
class Inventory;

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

private:
	Animation* mAnimation;
	Inventory* mInventory;
	float mHealth;
	float mMoveSpeed;
	float mAngle;
	float mCooldown;
	float mCounter;
};

#endif