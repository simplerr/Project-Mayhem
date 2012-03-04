#include <algorithm>
#include "Input.h"
#include "Graphics.h"
#include "Inventory.h"
#include "Item.h"
#include "enums.h"
#include "Player.h"
#include "Level.h"
#include "Loot.h"
#include "Item.h"

using namespace std;

Inventory::Inventory() : Container(400, 300, 675, 400)
{
	mGold = 0;
	mHooverBkgd = gGraphics->loadTexture("Data\\imgs\\hoover_bkgd.png");

	// Create the equip slots
	int x = getPosition().x - getWidth()/2 + 20;
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

		for(int i = 0; i < mSlotList.size() && 0; i++)
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

		sprintf(buffer, "Level: %i", mPlayer->getCharacterLevel());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 140, SMALL_DX);

		sprintf(buffer, "Experiece: %i/%i", mPlayer->getExperience(), mPlayer->getLevelExp());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 120, SMALL_DX);

		sprintf(buffer, "Gold: %i", mGold);
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 100, SMALL_DX);

		sprintf(buffer, "Health: %i", (int)mPlayer->getMaxHealth());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 80, SMALL_DX);

		sprintf(buffer, "MoveSpeed: %.2f", mPlayer->getMoveSpeed());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 60, SMALL_DX);

		sprintf(buffer, "Armor: %i", (int)mPlayer->getArmor());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 40, SMALL_DX);

		sprintf(buffer, "Energy: %i", (int)mPlayer->getMaxEnergy());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 20, SMALL_DX);
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

void Inventory::itemMoved(SlotItem* item, SlotId from, SlotId to)
{
	if(from == BAG && to != BAG)
		mPlayer->itemEquipped((Item*)item, true);
	else if(from != BAG && to == BAG)	{
		mPlayer->itemEquipped((Item*)item, false);
	}
}

void Inventory::addGold(int gold)
{
	mGold += gold;
}

void Inventory::itemOutsideSlot(SlotItem* item)
{
	Vector mousePos = gInput->mousePosition();

	// Outside the inventory? -> drop the item
	if(mousePos.x < getPosition().x - getWidth()/2 || mousePos.x > getPosition().x + getWidth()/2 || mousePos.y < getPosition().y - getHeight()/2 ||
		mousePos.y > getPosition().y + getHeight()/2)
	{
		Item* item1 = dynamic_cast<Item*>(item);
		mPlayer->getLevel()->addObject(new Loot(item1->getData().name, mousePos.x, mousePos.y));
		setMovingItem(NULL);
	}
	else
		Container::itemOutsideSlot(item);
}

ItemData Inventory::getAllStats()
{
	ItemData itemData;
	for(int i = 0; i < mSlotList.size(); i++)
	{
		// Don't count bag items
		if(mSlotList[i].slotId == BAG)
			continue;

		Item* item = (Item*)mSlotList[i].item;
		if(item != NULL) {
			itemData.armor += item->getData().armor;	
			itemData.health += item->getData().health;
			itemData.energy += item->getData().energy;
			itemData.weight += item->getData().weight;
			itemData.damage += item->getData().damage;
			itemData.moveSpeed += item->getData().moveSpeed;
		}
	}

	return itemData;
}

void Inventory::setPlayer(Player* player)
{
	mPlayer = player;
}