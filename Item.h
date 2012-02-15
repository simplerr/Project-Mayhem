#pragma once
#include <string>
#include "d3dUtil.h"
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

class Item
{
public:
	Item(string name);
	Item(){};
	virtual ~Item();

	void setId(int id);
	void setData(ItemData attributes);

	ItemData		getData();
	int				getId();
private:
	ItemData		mAttributes;
	int				mId;
};