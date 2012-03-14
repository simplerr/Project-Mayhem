#include "AI.h"
#include "Object.h"
#include "Vector.h"
#include "Math.h"


AI::AI(AIdata data)
{
	//flags.seen_enemy=false;
	if(data.fieldofview < 100)
		fov=(float)(data.fieldofview/30); //gives slightly above PI in vision
	else
		fov = (float)(100/30);

	visionRange=(float)(data.visionRange); 
	range = data.range;

	patrolSize = data.patrolareasize;
	int bravery = data.bravery;
	flags.init();
}

void AI::setTarget(Object *target) {
	ai_objectTarget= target;
	flags.seen_enemy= true;
}

Vector AI::getObjectTarget() {
	return ai_objectTarget->getPos();
}

Vector AI::getActionTarget() {
	return ai_actionTarget;
}

AI_action AI::getAction() {
	if(flags.seen_enemy)
	{
		return AI_MOVEATTACK;
	}
	else
	{
		if(flags.returnToOrigin) 
		{
			return AI_RETURN;
		}
		else
		{
			if(gMath->random(0,1000) < 4) {
				newTarget();
			}
			if(flags.patrol == true) 
			{
				return AI_PATROL;
			}

		return AI_IDLE;
		}
	}
}

void AI::newTarget(bool random, float rotation)
{
	facing = rotation;
	float v;
	if(random)
		v = gMath->random(0, 2*PI*360)/360;
	else
		v = rotation - PI + ((float)(gMath->random(0, (PI/20)*360)/360)-(PI/10));
	
	ai_actionTarget = Vector(cos(v), sin(v));
	if(random)
		ai_actionTarget.multiply(patrolSize);
	else
		ai_actionTarget.multiply(patrolSize/5);
	ai_actionTarget.add(ai_patrolOrigin);
	flags.patrol = true;
}