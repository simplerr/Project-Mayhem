#pragma once
#include <map>
#include "Item.h"

class ItemHandler
{
public:
	ItemHandler();
	~ItemHandler();

	ItemData getData(string name);
	map<string, ItemData> getDataMap();
private:
	map<string, ItemData> mDataMap;
};

extern ItemHandler* gItemHandler;