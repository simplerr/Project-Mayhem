#include <algorithm>
#include "Input.h"
#include "Graphics.h"
#include "Inventory.h"
#include "Item.h"

Inventory::Inventory()
{
	// Create the item handler, NOTE: Could be static since it's suppose to never be more than one!
	mMovingItem = NULL;
	mIdCounter = 0;
	mMovedFrom = 0;
	mPosition = Vector(400, 300);
	mWidth	= 500;
	mHeight = 400;
	mGold = 0;

	// Load textures
	mEquipSlot = gGraphics->loadTexture("Data\\imgs\\equip_slot.png");
	mBagSlot = gGraphics->loadTexture("Data\\imgs\\bag_slot.png");
	mBkgd = gGraphics->loadTexture("Data\\imgs\\inventory_bkgd.png");
	mHooverBkgd = gGraphics->loadTexture("Data\\imgs\\hoover_bkgd.png");

	// Create the equip slots
	int x = mPosition.x - mWidth/2;
	int y = mPosition.y - mHeight/2 + 35;
	mSlotList.push_back(Slot(x + 50, y + 50, SlotId::WEAPON));
	mSlotList.push_back(Slot(x + 120, y + 50, SlotId::SHIELD));
	mSlotList.push_back(Slot(x + 85, y + 130, SlotId::HEAD));
	mSlotList.push_back(Slot(x + 85, y + 200, SlotId::CHEST));
	mSlotList.push_back(Slot(x + 85, y + 270, SlotId::LEGS));

	// Create the bag slots
	x += 215;
	y += 50;
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
			mSlotList.push_back(Slot(x + 55*j, y + 55*i, BAG));

	// Not visible to start with
	hide();
}
	
Inventory::~Inventory()
{
	// Cleanup
	ReleaseCOM(mEquipSlot);
	ReleaseCOM(mBagSlot);
	ReleaseCOM(mBkgd);
	ReleaseCOM(mHooverBkgd);
}

void Inventory::update(float dt)
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
		// Equip if right clicked
		else if(gInput->keyPressed(VK_RBUTTON))
		{
			// Find out if a item was pressed
			for(int i = 0; i < mSlotList.size(); i++) 
			{
				// Pressed, not moving an item, item in the slot
				if(gMath->pointInsideRect(gInput->mousePosition(), mSlotList[i].rect) && mMovingItem == NULL && mSlotList[i].taken)
				{
					// TODO: Remove hacks!
					swapItems(&mSlotList[i], & mSlotList[mSlotList[i].item->getData().slot]);
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
				if(gMath->pointInsideRect(gInput->mousePosition(), mSlotList[i].rect) && (mSlotList[i].slotId == BAG || mSlotList[i].slotId == mMovingItem->getData().slot))
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

		// Hide inventory
		if(gInput->keyPressed('I'))
			hide();
	}
	else 
		// Show inventory
		if(gInput->keyPressed('I'))
			show();
}
	
void Inventory::draw()
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

			// A equip slot
			if(mSlotList[i].slotId != BAG) {
				gGraphics->drawTexture(mEquipSlot, mSlotList[i].rect);
				
				if(mSlotList[i].taken)
					gGraphics->drawTexture(mSlotList[i].item->getData().texture, rect.left + rect.getWidth()/2, rect.top + rect.getHeight()/2, rect.getWidth()-10, rect.getHeight()-10);	
			}
			// A bag slot
			else {
				gGraphics->drawTexture(mBagSlot, mSlotList[i].rect);
				if(mSlotList[i].taken)
					gGraphics->drawTexture(mSlotList[i].item->getData().texture, rect.left + rect.getWidth()/2, rect.top + rect.getHeight()/2, rect.getWidth()-5, rect.getHeight()-5);	
			}
		}

		// Draw the item information over all
		for(int i = 0; i < mSlotList.size(); i++)
		{
			Rect rect = mSlotList[i].rect;
			// Show item information?
			if(mSlotList[i].taken && mMovingItem == NULL && gMath->pointInsideRect(gInput->mousePosition(), rect)) 
			{
				Vector pos = Vector(rect.left + rect.getWidth()/2 + 100, rect.top + rect.getHeight()/2 + 125);

				int x = pos.x - rect.getWidth()/2 + - 60;
				int y = pos.y - rect.getHeight()/2 - 120;
				ItemData attributes = mSlotList[i].item->getData();
				gGraphics->drawTexture(mHooverBkgd, pos.x, pos.y, 200, 250);

				char buffer[256];
				if(attributes.damage != 0)
				{
					sprintf(buffer, "Damage: %i", attributes.damage);
					gGraphics->drawText(buffer, x, y+=30, SMALL_DX);
				}
				if(attributes.armor != 0)  
				{
					sprintf(buffer, "Armor: %i", attributes.armor);
					gGraphics->drawText(buffer, x, y+=30, SMALL_DX);
				}
				
				sprintf(buffer, "Weight: %i", attributes.weight);
				gGraphics->drawText(buffer, x, y+=30, SMALL_DX);
				// TODO: Slot..
				// TODO: Attack speed..
			}
		}

		// The moving item
		if(mMovingItem != NULL)
		{
			gGraphics->drawTexture(mMovingItem->getData().texture, gInput->mousePosition().x, gInput->mousePosition().y, 50, 50);
		}

		// Display gold amount
		char buffer[256];
		sprintf(buffer, "%i", mGold);
		gGraphics->drawText(buffer, mPosition.x + 50, mPosition.y + 200, SMALL_DX);
	}
}

void Inventory::swapItems(Slot* from, Slot* to)
{
	Slot tmp = *from;
	if((*to).taken) {
		(*from).item = (*to).item;
		(*from).taken = true;
	}
	else
		(*from).taken = false;

	(*to).item = tmp.item;
	(*to).taken = true;
}

void Inventory::show()
{
	mVisible = true;
}
	
void Inventory::hide()
{
	mVisible = false;
}

void Inventory::addItem(string itemName)
{
	Item* item = new Item(itemName);
	item->setId(mIdCounter);
	item->setData(gItemHandler->getData(itemName));

	// Find first free slot
	for(int i = 0; i < mSlotList.size(); i++) {
		if(!mSlotList[i].taken && mSlotList[i].slotId == BAG) {
			mSlotList[i].item = item;
			mSlotList[i].taken = true;
			break;
		}
	}
}

void Inventory::addGold(int gold)
{
	mGold += gold;
}
	
void Inventory::removeItem(Item item)
{
	return;

	// TODO...

	/*int index = 0;
	for(int i = 0; i < mItemList.size(); i++)
		if(mItemList[i].getId() == item.getId())
			index = i;

	mItemList.erase(std::remove(mItemList.begin(), mItemList.end(), mItemList[index]), mItemList.end());*/
}