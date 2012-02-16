#pragma once
#include <string>
#include "d3dUtil.h"
#include "SlotItem.h"
using namespace std;

struct ItemData
{
	string name;
	int armor;
	int health;
	int damage;
	int mana;
	int weight;
	int slot;
	IDirect3DTexture9* texture;
	string textureSource;
	// Etc..
};

class Item : public SlotItem
{
public:
	Item(string name);
	Item(){};
	~Item();

	void setData(ItemData attributes);

	ItemData		getData();
	int				getId();
	SlotId			getSlotId();
	IDirect3DTexture9* getTexture();
private:
	ItemData		mAttributes;
};