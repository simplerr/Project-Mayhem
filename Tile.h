#ifndef TILE_H
#define TILE_H

#include <string>
#include "Vector.h"

struct TileData;

using namespace std;

// :NOTE: Could as well be a struct
class Tile
{
public:
	Tile(int x, int y, string name);
	Tile();
	~Tile();
	
	void		setData(TileData* data);
	void		setName(string name);
	void		setPosition(Vector pos);
	TileData*	getData();
	string		getName();
	Vector		getPosition();
private:
	TileData*	mData;
	Vector		mPos;
};

#endif