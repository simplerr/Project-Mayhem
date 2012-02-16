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
			if(gMath->random(0,1000) < 5) {
				float v = gMath->random(0, 2*PI*360)/360;
				ai_actionTarget = Vector(cos(v), sin(v));
				ai_actionTarget.multiply(patrolSize);
				ai_actionTarget.add(ai_patrolOrigin);
				flags.patrol = true;
			}
			if(flags.patrol == true) 
			{
				return AI_PATROL;
			}
		
		return AI_IDLE;
		}
	}
}