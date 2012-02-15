#include "Tile.h"
#include "TileHandler.h"

Tile::Tile(int x, int y, string name)
{
	mData = NULL;
	setPosition(Vector(x, y));
}

Tile::Tile()
{
	setData(NULL);
}

Tile::~Tile()
{
	
}

void Tile::setName(string name)
{
	mData->name = name;
}

void Tile::setPosition(Vector pos)
{
	mPos = pos;
}

void Tile::setData(TileData* data)
{
	mData = data;
}

string Tile::getName()
{
	return mData->name;
}

TileData* Tile::getData()
{
	return mData;
}

Vector Tile::getPosition()
{
	return mPos;
}