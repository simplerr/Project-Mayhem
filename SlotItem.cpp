#include "SlotItem.h"

SlotItem::SlotItem()
{

}

SlotItem::~SlotItem()
{

}

void SlotItem::setId(int id)
{
	mId = id;
}

void SlotItem::setSlotId(SlotId id)
{
	mSlotId = id;
}

int SlotItem::getId()
{
	return mId;
}

SlotId SlotItem::getSlotId()
{
	return mSlotId;
}