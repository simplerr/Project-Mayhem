#pragma once

#include <string>
#include <map>

class Enemy;
class Object;
class Region;

enum Event {
	PLAYER_IN_RECT,
	ENEMY_IN_RECT,
	PERIODICALLY,
	AFTER_TIME_ELAPSED,
	ON_ENEMY_DEATH
};

//enum VariableType {
//	VT_INTEGER,
//	VT_STRING,
//	VT_BOOL,
//	VT_ENEMY
//};
//
//struct localVariable
//{
//	VariableType type;
//
//	int i;
//	string s;
//	bool b;
//	Enemy* e;
//	void setType(string str) {
//		if (str == "i")
//			type = VT_INTEGER;
//		else if (str == "b")
//			type = VT_BOOL;
//		else if  (str == "e")
//			type == VT_ENEMY;
//		else if (str == "s")
//			type = VT_STRING;
//		else 
//			type = VT_INTEGER;
//	}
//	
//};
struct localVariable;

class Trigger
{
public:
	Trigger(std::string action, Event eventType, Region* owner);
	Trigger();
	~Trigger();

	void doActions(Object* causingObject);
	Event getEvent()	{return mEventType;}
private:
	Event mEventType;
	std::map<std::string, std::string> mLocalVars;
	//std::string mAction[256][256];
	Region* mOwner;
};