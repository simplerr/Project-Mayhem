#pragma once
#include <string>
#include "d3dUtil.h"
#include "SlotItem.h"
using namespace std;

struct ItemData
{
	ItemData() {
		armor = health = energy = weight = damage = moveSpeed = 0;
	}
	string name;
	int armor;
	int health;
	int damage;
	int energy;
	int weight;
	int slot;
	float moveSpeed;
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
	SlotId			getSlotId();
	IDirect3DTexture9* getTexture();
private:
	ItemData		mAttributes;
};