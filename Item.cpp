#include "Item.h"

Item::Item(string name)
{
	// Load attributes
}
	
Item::~Item()
{

}

void Item::setId(int id)
{
	mId = id;
}

void Item::setData(ItemData attributes)
{
	mAttributes = attributes;
}
	
ItemData Item::getData()
{
	return mAttributes;
}

int Item::getId()
{
	return mId;
}