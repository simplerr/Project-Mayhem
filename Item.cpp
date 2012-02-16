#include "Item.h"

Item::Item(string name) : SlotItem()
{
	// Load attributes
}
	
Item::~Item()
{

}

void Item::setData(ItemData attributes)
{
	mAttributes = attributes;
}
	
ItemData Item::getData()
{
	return mAttributes;
}

IDirect3DTexture9* Item::getTexture()
{
	return mAttributes.texture;
}

SlotId Item::getSlotId()
{
	return (SlotId)mAttributes.slot;
}