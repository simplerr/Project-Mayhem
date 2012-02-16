#pragma once
#include <vector>
#include "Vector.h"
#include "ItemHandler.h"
#include "Rect.h"
#include "Container.h"

using namespace std;

class Item;

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

	void update(float dt);
	void draw();

	void addItem(string itemName);
	void addGold(int gold = 1);
	void removeItem(Item item);

private:
	IDirect3DTexture9*	mHooverBkgd;
	int					mGold;
};