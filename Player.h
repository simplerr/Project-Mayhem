#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Animation.h"
#include "Collision.h"
#include "Weapon.h"

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

	void addExperience(int experience);
	void addGold(int amount);

	float getMoveSpeed();
	int getHealth();
	int getMaxHealth();
	int getEnergy();
	int getMaxEnergy();
	int getArmor();
	int getExperience();
	int getCharacterLevel();
	int	getLevelExp();	
	bool inInventory();

	//void setEnergy(float nrg)	{mEnergy = nrg;}
	void costEnergy(float dnrg)	{mEnergy-=dnrg;}
	void damage(float dmg);
private:
	Animation*	mAnimation;
	Inventory*	mInventory;
	Weapon*		mWeapon;
	Gui*		mGui;
	vector<int> mExpPerLevel;
	float mAngle;
	float mCooldown;
	float mCounter;

	// Stats
	int	mLevel;
	int mExperience;
	float mHealth;
	float mEnergy;
	float mMaxHealth;
	float mMaxEnergy;
	float mArmor;
	float mBaseArmor;
	float mMoveSpeed;
};

#endif