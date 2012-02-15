#pragma once
#include <vector>
#include "Vector.h"
#include "ItemHandler.h"
#include "Rect.h"

using namespace std;

class Item;

// Enum for different type of slots
enum SlotId {WEAPON, SHIELD, HEAD, CHEST, LEGS, MISC1, MISC2, BAG};

// Represents a slot in the inventory 
struct Slot
{
	Slot(int x, int y, SlotId id) {
		slotId = id;
		int size = 50;
		taken = false;
		rect.left = x - size/2;
		rect.right = x + size/2;
		rect.top = y - size/2;
		rect.bottom = y + size/2;
	}
	SlotId	slotId;
	Item*	item;
	bool	taken;
	Rect	rect;
};

class ItemTooltip
{
public:
	ItemTooltip();
	~ItemTooltip();

	void draw();


};

// The players inventory
class Inventory
{
public:
	Inventory();
	virtual ~Inventory();

	virtual void update(float dt);
	virtual void draw();

	void show();
	void hide();

	void addItem(string itemName);
	void addGold(int gold = 1);
	void removeItem(Item item);
	void swapItems(Slot* from, Slot* to);
private:
	IDirect3DTexture9*  mEquipSlot;
	IDirect3DTexture9*  mBagSlot;
	IDirect3DTexture9*	mBkgd;
	IDirect3DTexture9*	mHooverBkgd;
	std::vector<Slot>	mSlotList;
	Item*				mMovingItem;
	int					mMovedFrom;
	int					mIdCounter;
	bool				mVisible;

	Vector				mPosition;
	int					mWidth, mHeight;
	int					mGold;
};