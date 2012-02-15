#ifndef TILEHANDLER_H
#define TILEHANDLER_H

#include <vector>
#include <map>
#include "d3dUtil.h"

using namespace std;

struct TileData;

// For readability
typedef map<string, TileData*>::iterator TileIter;
typedef pair<string, TileData*> TilePair;

struct TileData
{
	IDirect3DTexture9*	texture;
	string				name;
	string				textureSource;
	bool				obstacle;
	float				friction;
};

class TileHandler 
{
public:
	TileHandler();
	~TileHandler();

	void loadTiles(string source);

	map<string, TileData*>	getDataMap();
	TileData*				getData(string name);
	
private:
	map<string, TileData*>	mDataMap;
};

#endif

