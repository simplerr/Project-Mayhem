#include "AI.h"
#include "Object.h"
#include "Vector.h"

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
	flagAI.init();
}

void AI::setTarget(Object *target) {
	ai_target= target;
	flagAI.seen_enemy= true;
}

Vector AI::getTargetPos() {
	return ai_target->getPos();
}

AI_action AI::getAction() {
	if(flagAI.seen_enemy)
	{
		return AI_MOVEATTACK;
	}
	else
	{
		if(flagAI.returnToOrigin) 
		{
			return AI_RETURN;
		}
		return AI_IDLE;
	}
}