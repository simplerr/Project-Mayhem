#include <algorithm>
#include "Input.h"
#include "Graphics.h"
#include "Inventory.h"
#include "Item.h"
#include "enums.h"

using namespace std;

Inventory::Inventory() : Container(400, 300, 500, 400)
{
	mGold = 0;
	mHooverBkgd = gGraphics->loadTexture("Data\\imgs\\hoover_bkgd.png");

	// Create the equip slots
	int x = getPosition().x - getWidth()/2;
	int y = getPosition().y - getHeight()/2 + 35;
	
	addSlot(x + 50, y + 50, SlotId::WEAPON);
	addSlot(x + 120, y + 50, SlotId::SHIELD);
	addSlot(x + 85, y + 130, SlotId::HEAD);
	addSlot(x + 85, y + 200, SlotId::CHEST);
	addSlot(x + 85, y + 270, SlotId::LEGS);

	// Create the bag slots
	x += 215;
	y += 50;
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
			addSlot(x + 55*j, y + 55*i, BAG);

	// Not visible to start with
	hide();
}
	
Inventory::~Inventory()
{
	// Cleanup
	ReleaseCOM(mHooverBkgd);
}

void Inventory::update(float dt)
{
	// Visible?
	if(getVisible())
	{
		Container::update(dt);
		// Equip if right clicked
		if(gInput->keyPressed(VK_RBUTTON))
		{
			// Find out if a item was pressed
			for(int i = 0; i < mSlotList.size(); i++) 
			{
				// Pressed, not moving an item, item in the slot
				if(gMath->pointInsideRect(gInput->mousePosition(), mSlotList[i].rect) && getMovingItem() == NULL && mSlotList[i].taken)
				{
					// TODO: Remove hacks!
					int iii = mSlotList[i].item->getSlotId();
					swapItems(&mSlotList[i], & mSlotList[mSlotList[i].item->getSlotId()]);
					break;
				}
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
	if(getVisible())
	{
		Container::draw();

		// Draw the item information overlay
		for(int i = 0; i < mSlotList.size(); i++)
		{
			Rect rect = mSlotList[i].rect;
			// Show item information?
			if(mSlotList[i].taken && getMovingItem() == NULL && gMath->pointInsideRect(gInput->mousePosition(), rect)) 
			{
				Vector pos = Vector(rect.left + rect.getWidth()/2 + 100, rect.top + rect.getHeight()/2 + 125);

				int x = pos.x - rect.getWidth()/2 + - 60;
				int y = pos.y - rect.getHeight()/2 - 120;
				ItemData attributes = ((Item*)mSlotList[i].item)->getData();
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

		// Display gold amount
		char buffer[256];
		sprintf(buffer, "%i", mGold);
		gGraphics->drawText(buffer, getPosition().x + 50, getPosition().y + 200, SMALL_DX);
	}
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