#pragma once
#include <map>
#include "Item.h"
#include "Object.h"

class ObjectHandler
{
public:
	ObjectHandler();
	~ObjectHandler();

	ObjectData getData(string name);
	map<string, ObjectData> getDataMap();
private:
	map<string, ObjectData> mDataMap;
};

extern ObjectHandler* gObjectHandler;