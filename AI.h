#ifndef AI_H
#define AI_H

#include <vector>
#include "Vector.h"

class Object;

enum AI_action 
{
	AI_MOVEATTACK = 0,
	AI_HIDE,
	AI_ATTACK,
	AI_IDLE,
	AI_PATROL,
	AI_RETURN,
	AI_PANIC
};

struct AIflags {
	bool seen_enemy;
	bool returnToOrigin;

	void init() {
		seen_enemy = false;
		returnToOrigin = false;
	}
};

struct AIdata {
	short int visionRange;
	short int fieldofview;
	short int bravery;
	short int range;
	/*short int intelegence;
	short int reactiveness;*/
	float patrolareasize;
	void init() {
		fieldofview=30;
		visionRange=200;
		bravery = 3;
		patrolareasize = 100;
		range = 100;
	}
};

class AI
{
public:
	AI(AIdata data);
	AI(){};
	~AI(){};

	void setPatrolOrigin(Vector v) {ai_patrolOrigin = v;}
	void setTarget(Object *target);
	
	float getVisionRange()		{return visionRange;}
	float getRange()			{return range;}
	Vector getPatrolPos()		{return ai_patrolOrigin;}
	Vector getTargetPos();
	AI_action getAction();

	bool seenEnemy()			{return flagAI.seen_enemy;}
	void setSeenEnemy(bool b)	{flagAI.seen_enemy = b;}

	AIflags flagAI;

private:
	Object *ai_target;
	Vector ai_patrolOrigin;
	float patrolSize;
	std::vector<AI_action> ai_list;
	
	float facing;
	float fov;
	float visionRange;
	float range;
	int bravery;
	float ai_timer;
};

#endif