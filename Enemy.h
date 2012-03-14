#ifndef ENEMY_H
#define ENEMY_H

#include "d3dUtil.h"
#include "Object.h"
#include "Collision.h"
#include "Animation.h"
#include "AI.h"
#include <map>
#include <string>

using namespace std;
class Animation;

class EnemyData 
{
public:
	EnemyData(int w, int h, string n, string texture);
	EnemyData() {};
	~EnemyData();

	string name;
	string textureSource;
	int experience;
	int hp; 
	int range;
	int height;
	int width;
	int speed;
	int weaponRate;

	/* A I   D A T A*/
	AIdata ai_data;
};


//Handler
class Enemies
{
public:
	Enemies() {init();}
	~Enemies();
	EnemyData* getData(string searchFor); 
	std::map<string, EnemyData*> getDataMap() { return data;}
	
	//!Run to load vector with predefined data
	void init(); 
private:
	std::map<string, EnemyData*> data; 
	std::map<string, EnemyData*>::iterator mIt; 
	
};


class Enemy : public Object
{
public:
	//!Use getData to easily find EnemyData
	//!All EnemyDataes are located in the Types map class
	Enemy(float x, float y, EnemyData* type); //ptr
	~Enemy();

	void update(float dt);
	void draw();
	bool handleCollision(Object* collider, MTV* mtv);

	EnemyData* getClass()			{return mClass;}
	float getHP()					{return mHp;}
	float getRange()				{return mRange;}
	float getSpeed()				{return mSpeed;}
	float getExperience()			{return mExperience;}

	void setClass(EnemyData* Class)	{mClass = Class;} //ptr
	void setHP(float HP)				{mHp = HP;}
	void setAI(AI* new_ai)				{ai = new_ai;}
	void setSpeed(float speed)			{mSpeed = speed;}
	void setExperience(int exp)			{mExperience = exp;}

	void modHP(float dHP)				{mHp+=dHP;}
	void damage(float dHP);

	void doAI(float dt);
	void calcAI(float dt);
	void initAI(AIdata data, Vector v);
	bool turns()						{return mTurns;}
private:
	bool mTurns;
	EnemyData* mClass;
	Animation* mAnimation;
	AI* ai;
	float mHp;
	float mRange;
	float mSpeed;
	float mWeaponRate;
	int mExperience;
	
	//Timers
	float mTime;
	float mAttackTimer;
	float mLostSightTimer;
};

extern Enemies* gEnemies;

#endif