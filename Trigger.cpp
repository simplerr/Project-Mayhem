#include <string>
#include "Trigger.h"
#include "Region.h"
#include "WonState.h"
#include "GameState.h"
#include <boost\algorithm\string.hpp>
#include "Game.h"


//!Triggers looks as followed: 
//!ACTION1 PARAM1 PARAM2; ACTION2 PARAM1 PARAM2;
Trigger::Trigger(std::string action, Event eventType, Region* owner)
{
	mEventType = eventType;
	mOwner = owner;
	std::vector<std::string> tv;
	std::vector<std::string> tu;
	Action a;
	if(action!="") 
	{
		boost::split(tv, action, boost::is_any_of(";"));
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

Trigger::Trigger() 
{
	mEventType = PLAYER_IN_RECT;
}
Trigger::~Trigger()
{

}

void Trigger::doActions(Object* causingObject)
{
	for(int i = 0; i < mActions.size(); i++) {
		if(mActions.at(i).action == "Win")
			gGameState->changeState(WonState::Instance());
		else
			MessageBox(0,"Something happened!",0,0);
	}
}