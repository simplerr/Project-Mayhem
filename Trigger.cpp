#include <string>
#include "Trigger.h"
#include "Region.h"


Trigger::Trigger(std::string action, Event eventType, Region* owner)
{
	//split string action

	mEventType = eventType;
	mOwner = owner;
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
	MessageBox(0,"Workin'",0,0);
}