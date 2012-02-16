#include <algorithm>
#include "Input.h"
#include "Graphics.h"
#include "Container.h"
#include "Item.h"

Container::Container(int x, int y , int width, int height)
{
	// Create the item handler, NOTE: Could be static since it's suppose to never be more than one!
	mMovingItem = NULL;
	mIdCounter = 0;
	mMovedFrom = 0;
	mPosition = Vector(x, y);
	mWidth	= width;
	mHeight = height;

	// Load textures
	mEquipSlot = gGraphics->loadTexture("Data\\imgs\\equip_slot.png");
	mBagSlot = gGraphics->loadTexture("Data\\imgs\\bag_slot.png");
	mBkgd = gGraphics->loadTexture("Data\\imgs\\inventory_bkgd.png");
	mSkillSlot = gGraphics->loadTexture("Data\\imgs\\skill_slot.png");

	// Not visible to start with
	hide();
}
	
Container::~Container()
{
	// Cleanup
	ReleaseCOM(mEquipSlot);
	ReleaseCOM(mBagSlot);
	ReleaseCOM(mBkgd);
	ReleaseCOM(mSkillSlot);
}

void Container::update(float dt)
{
	// Visible?
	if(mVisible)
	{
		// Move if left clicked
		if(gInput->keyPressed(VK_LBUTTON))
		{
			// Find out if a item was pressed
			for(int i = 0; i < mSlotList.size(); i++) 
			{
				// Pressed, not moving an item, item in the slot
				if(gMath->pointInsideRect(gInput->mousePosition(), mSlotList[i].rect) && mMovingItem == NULL && mSlotList[i].taken)
				{
					mSlotList[i].taken = false;
					mMovingItem = mSlotList[i].item;
					mMovedFrom = i;
					break;
				}
			}
		}

		// Left mouse button released
		if(gInput->keyReleased(VK_LBUTTON) && mMovingItem != NULL)
		{
			bool any = false;
			for(int i = 0; i < mSlotList.size(); i++) 
			{
				// Inside any slot?
				if(gMath->pointInsideRect(gInput->mousePosition(), mSlotList[i].rect) && (mSlotList[i].slotId == BAG || mSlotList[i].slotId == mMovingItem->getSlotId()))
				{
					swapItems(&mSlotList[mMovedFrom], &mSlotList[i]);
					mMovingItem = NULL;
					any = true;
				}
			}

			// Not inside a slot, move back to original slot
			if(!any) {
				mSlotList[mMovedFrom].taken = true;
				mMovingItem = NULL;
			}
		}
	}
}
	
void Container::draw()
{
	// Visible?
	if(mVisible)
	{
		// Background
		gGraphics->drawTexture(mBkgd, mPosition.x, mPosition.y, mWidth, mHeight);

		// Loop through all slots
		for(int i = 0; i < mSlotList.size(); i++)
		{
			Rect rect = mSlotList[i].rect;

			IDirect3DTexture9* texture;

			if(mSlotList[i].slotId == BAG)
				texture = mBagSlot;
			else if(mSlotList[i].slotId == SKILL)
				texture = mSkillSlot;
			else
				texture = mEquipSlot;

			gGraphics->drawTexture(texture, mSlotList[i].rect);
				
			if(mSlotList[i].taken)
				gGraphics->drawTexture(mSlotList[i].item->getTexture(), rect.left + rect.getWidth()/2, rect.top + rect.getHeight()/2, rect.getWidth()-10, rect.getHeight()-10);
		}

		// The moving item
		if(mMovingItem != NULL)
		{
			gGraphics->drawTexture(mMovingItem->getTexture(), gInput->mousePosition().x, gInput->mousePosition().y, 50, 50);
		}
	}
}

void Container::swapItems(Slot* from, Slot* to)
{
	itemMoved((*from).item, from->slotId, to->slotId);
	// Ugly swapping method.. Also checks if one slot is empty or not
	Slot tmp = *from;
	if((*to).taken) {
		itemMoved((*to).item, to->slotId, from->slotId);
		(*from).item = (*to).item;
		(*from).taken = true;
	}
	else
		(*from).taken = false;

	(*to).item = tmp.item;
	(*to).taken = true;
}

void Container::setTexture(string texture)
{
	mBkgd = gGraphics->loadTexture(texture);
}

void Container::show()
{
	mVisible = true;
}
	
void Container::hide()
{
	mVisible = false;
}

void Container::addSlot(int x, int y, SlotId id)
{
	mSlotList.push_back(Slot(x, y, id));
}

void Container::setSlotSize(int size)
{
	mSlotSize = size;
}

Vector Container::getPosition()
{
	return mPosition;
}

SlotItem* Container::getMovingItem()
{
	return mMovingItem;
}
	
int Container::getWidth()
{
	return mWidth;
}
	
int Container::getHeight()
{
	return mHeight;
}

bool Container::getVisible()
{
	return mVisible;
}