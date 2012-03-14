#pragma once
#include <vector>
#include "Rect.h"
#include "Vector.h"
#include "SlotItem.h"
#include "enums.h"
using namespace std;

//! Represents a slot in any container
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
	SlotId		slotId;
	SlotItem*	item;
	bool		taken;
	Rect		rect;
};

//! Base class for all containers. Invetery, skillbar, chests etc.. should inherit from this class.
//! Holds a list of slots with items in them. Handles item movement and swapping withing the container.
class Container
{
public:
	Container(int x, int y, int width, int height);
	virtual ~Container();

	virtual void update(float dt);
	virtual void draw();
	void drawSlots();
	void drawItems();
	virtual void itemMoved(SlotItem* item, SlotId from, SlotId to){};
	virtual void itemOutsideSlot(SlotItem* item);
	virtual void selectedMovingItem(SlotItem* item) {};
	virtual void releasedMovingItem(SlotItem* item) {};

	//virtual void addItem(string itemName);
	void addSlot(int x, int y, SlotId id);
	//void removeItem(SlotItem item);
	void swapItems(Slot* slot1, Slot* slot2);

	void show();
	void hide();

	void setTexture(string texture);
	void setSlotSize(int size = 40);
	void setMovingItem(SlotItem* item) {mMovingItem = item;}

	SlotItem* getMovingItem();
	Vector	getPosition();
	int		getWidth();
	int		getHeight();
	bool	getVisible();

	std::vector<Slot>	mSlotList;
	int					mIdCounter;
private:
	IDirect3DTexture9*  mEquipSlot;
	IDirect3DTexture9*  mBagSlot;
	IDirect3DTexture9*	mSkillSlot;
	IDirect3DTexture9*	mBkgd;
	IDirect3DTexture9*	mSkillBG;
	bool				mVisible;
	Vector				mPosition;
	int					mWidth, mHeight;
	SlotItem*			mMovingItem;
	int					mMovedFrom;
	int					mSlotSize;
};