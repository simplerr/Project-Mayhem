#include <string>
#include "Trigger.h"
#include "Region.h"
#include "WonState.h"
#include "GameState.h"
#include "Player.h"
#include "Enemy.h"
#include <boost\algorithm\string.hpp>
#include "Game.h"
#include "Level.h"

/*
Triggerlist:
Action:   Parameters:
 Win    | 
 Damage | DamagePerSecond
*/


//!Triggers looks as followed: 
//!ACTION1 PARAM1 PARAM2; ACTION2 PARAM1 PARAM2;
Trigger::Trigger(std::string action, Event eventType)
{
	mEventType = eventType;
	mTrigger=action;
	setTriggerString(action);
}

Trigger::Trigger() 
{
	mEventType = PLAYER_IN_RECT;
}
Trigger::~Trigger()
{

}

void Trigger::setTriggerString(std::string s) 
{
	mTrigger=s;
	std::vector<std::string> tv;
	std::vector<std::string> tu;
	Action a;
	if(s!="") 
	{
		boost::split(tv, s, boost::is_any_of(";"));
		for(int i = 0; i < tv.size(); i++) 
		{
			boost::split(tu, tv.at(i), boost::is_any_of(" "));
			if(i == 0) {
				a.action = tu.at(i);
			}
			else {
				a.param[i-1] = tu.at(i);
			}
		}
		mActions.push_back(a);
	}
	else {
		mActions.push_back(a);
	}
}

void Trigger::doActions(Object* causingObject, float dt)
{
	for(int i = 0; i < mActions.size(); i++) {
		if(mActions.at(i).action == "Win") {
			getOwner()->getLevel()->changeState(true);
			gGameState->changeState(WonState::Instance());
		}
		else if(mActions.at(i).action == "Damage") {
			if(causingObject->getType() == PLAYER)
				dynamic_cast<Player*>(causingObject)->damage(atoi(mActions.at(i).param[1].c_str())*dt);
			else 
				dynamic_cast<Enemy*>(causingObject)->damage(atoi(mActions.at(i).param[1].c_str())*dt);
		}
		else
			MessageBox(0,"Something happened!",0,0);
	}
}