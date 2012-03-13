#pragma once
#include <vector>
#include "Vector.h"
#include "ItemHandler.h"
#include "Rect.h"
#include "Container.h"

using namespace std;

class Item;
class Player;

class ItemTooltip
{
public:
	ItemTooltip();
	~ItemTooltip();

	void draw();
};

// The players inventory
class Inventory : public Container
{
public:
	Inventory();
	virtual ~Inventory();

	void itemOutsideSlot(SlotItem* item);
	void setPlayer(Player* player);
	void update(float dt);
	void draw();
	void itemMoved(SlotItem* item, SlotId from, SlotId to);
	void addItem(string itemName, SlotId slotId = BAG);
	void addGold(int gold = 1);
	void removeItem(Item item);
	ItemData getAllStats();

private:
	IDirect3DTexture9*	mHooverBkgd;
	IDirect3DTexture9*  mCloseTexture;
	Rect				mCloseRect;
	Player*				mPlayer;
	int					mGold;
};