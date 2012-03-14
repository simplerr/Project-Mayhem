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

Inventory::Inventory() : Container(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 750, 400)
{
	mGold = 0;
	mHooverBkgd = gGraphics->loadTexture("Data\\imgs\\SkillBG.png");
	mCloseTexture = gGraphics->loadTexture("Data\\imgs\\close_button.png");

	// Create the equip slots
	int x = getPosition().x - getWidth()/2 + 20;
	int y = getPosition().y - getHeight()/2 + 35;

	mCloseRect = Rect(getPosition().x + getWidth()/2 - 20, getPosition().y - getHeight()/2 + 20, 30, 30, 1);
	
	addSlot(x + 85, y + 50, SlotId::WEAPON);
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

	mCorrespondingSlot = NULL;
}
	
Inventory::~Inventory()
{
	// Cleanup
	ReleaseCOM(mHooverBkgd);
	ReleaseCOM(mCloseTexture);
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
					// NOTE: Hack code.. hope it doesn't break!
					if(mSlotList[i].slotId == mSlotList[i].item->getSlotId()) {
						Item* item = (Item*)mSlotList[i].item;
						mPlayer->itemEquipped((Item*)mSlotList[i].item, false);
						addItem(item->getData().name);
						mSlotList[i].item = NULL;
						mSlotList[i].taken = false;
					}
					else {
						int id = -1;
						bool taken = false;
						for(int j = 0; j < mSlotList.size(); j++) 
							if(mSlotList[j].slotId == mSlotList[i].item->getSlotId()) {
								id = j;
								if(mSlotList[j].taken)
									taken = true;
								break;
							}

						if(taken) {
							// The equip slot was taken
							itemMoved(mSlotList[id].item, mSlotList[id].slotId, mSlotList[i].slotId);
							itemMoved(mSlotList[i].item, mSlotList[i].slotId, mSlotList[id].slotId);
							SlotItem* tmp = mSlotList[i].item;
							mSlotList[i].item = mSlotList[id].item;
							mSlotList[id].item = tmp;
							
						}
						else {
							// The equip slot was not taken
							itemMoved(mSlotList[i].item, mSlotList[i].slotId, mSlotList[id].slotId);
							mSlotList[id].item = mSlotList[i].item;
							mSlotList[id].taken = true;
							mSlotList[i].taken = false;
						}
					}
					break;
				}
			}
		}

		// Hide inventory
		if(gInput->keyPressed('I') || gInput->keyPressed('B') || gInput->keyPressed(VK_TAB) || gInput->keyPressed(VK_ESCAPE))
			hide();
		if(gInput->keyPressed(VK_LBUTTON) && gMath->pointInsideRect(gInput->mousePosition(), mCloseRect))
			hide();
	}
	else 
		// Show inventory
		if(gInput->keyPressed('I') || gInput->keyPressed('B') || gInput->keyPressed(VK_TAB))
			show();
}
	
void Inventory::draw()
{
	// Visible?
	if(getVisible())
	{
		drawSlots();
		if(mCorrespondingSlot != NULL) {
			Rect rect = mCorrespondingSlot->rect;
			rect.left += 5;
			rect.right -= 5;
			rect.top += 5;
			rect.bottom -= 5;
			gGraphics->drawRect(rect, 0xff00ff00);
		}
		
		drawItems();
		char buffer[256];

		sprintf(buffer, "Level: %i", mPlayer->getCharacterLevel());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 140, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));

		sprintf(buffer, "Experiece: %i/%i", mPlayer->getExperience(), mPlayer->getLevelExp());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 120, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));

		sprintf(buffer, "Gold: %i", mGold);
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 100, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));;

		sprintf(buffer, "Health: %i", (int)mPlayer->getMaxHealth());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 80, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));

		sprintf(buffer, "MoveSpeed: %.2f", mPlayer->getMoveSpeed());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 60, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));

		sprintf(buffer, "Armor: %i", (int)mPlayer->getArmor());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 40, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));

		sprintf(buffer, "Energy: %i", (int)mPlayer->getMaxEnergy());
		gGraphics->drawText(buffer, getPosition().x + 160, getPosition().y - 20, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));

		gGraphics->drawTexture(mCloseTexture, mCloseRect);

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
				gGraphics->drawTexture(mHooverBkgd, pos.x, pos.y-75, 200, 100);

				char buffer[256];
				if(attributes.damage != 0)
				{
					sprintf(buffer, "Damage: %i", attributes.damage);
					gGraphics->drawText(buffer, x, y+=30, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));
				}
				if(attributes.armor != 0)  
				{
					sprintf(buffer, "Armor: %i", attributes.armor);
					gGraphics->drawText(buffer, x, y+=30, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));
				}
				
				sprintf(buffer, "Weight: %i", attributes.weight);
				gGraphics->drawText(buffer, x, y+=30, CUSTOM, 10, D3DCOLOR_RGBA(255,128,0,255));
				// TODO: Slot..
				// TODO: Attack speed..
			}
		}
	}
}

void Inventory::addItem(string itemName, SlotId slotId)
{
	Item* item = new Item(itemName);
	item->setId(mIdCounter);
	item->setData(gItemHandler->getData(itemName));

	if(slotId == BAG) 
	{
		// Find first free slot
		for(int i = 0; i < mSlotList.size(); i++) {
			if(!mSlotList[i].taken && mSlotList[i].slotId == BAG) {
				mSlotList[i].item = item;
				mSlotList[i].taken = true;
				break;
			}
		}
	}
	else
	{
		for(int i = 0; i < mSlotList.size(); i++) {
			if(!mSlotList[i].taken && mSlotList[i].slotId == slotId) {
				mSlotList[i].item = item;
				mSlotList[i].taken = true;
				mPlayer->itemEquipped(item, true);
				break;
			}
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

void Inventory::selectedMovingItem(SlotItem* item)
{
	if(item->getSlotId() != BAG)
		for(int i = 0; i < mSlotList.size(); i++)
			if(mSlotList[i].slotId == item->getSlotId())
				mCorrespondingSlot = &mSlotList[i];
}
	
void Inventory::releasedMovingItem(SlotItem* item)
{
	mCorrespondingSlot = NULL;
}