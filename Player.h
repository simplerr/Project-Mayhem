#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Animation.h"
#include "Collision.h"

class Level;
class Inventory;
class Gui;
class Item;

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
	bool handleCollision(Object* collider, MTV* mtv);
	void itemEquipped(Item* item, bool equiped);

	void setCooldown(float cooldown);

	float getHealth();
	float getMaxHealth();
	float getEnergy();
	float getMaxEnergy();

private:
	IDirect3DTexture9*	mWeapon;
	Animation*	mAnimation;
	Inventory*	mInventory;
	Gui*		mGui;
	float mAngle;
	float mCooldown;
	float mCounter;

	// Stats
	float mHealth;
	float mEnergy;
	
	// HACK::::::
public:
	float mMaxHealth;
	float mMaxEnergy;
	float mArmor;
	float mBaseArmor;
	float mMoveSpeed;
};

#endif