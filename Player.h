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
class StatusText;

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
	void setHealth(int hp);
	void setEnergy(int energy);

	float getMoveSpeed();
	float getHealth();
	int getMaxHealth();
	float getEnergy();
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
	StatusText*	mStatusText;
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