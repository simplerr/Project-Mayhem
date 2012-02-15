#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
#include "TileHandler.h"
#include "Graphics.h"

TileHandler::TileHandler()
{
	// Default constructor
}

TileHandler::~TileHandler()
{
	// :TODO: Cleanup tile data

	for(TileIter iter = mDataMap.begin(); iter != mDataMap.end(); iter++)
	{
		TileData* data = iter->second;
		delete data;
	}

	mDataMap.clear();
}

void TileHandler::loadTiles(string source)
{
	// Open and load the XML document
	TiXmlDocument doc = TiXmlDocument(source.c_str());
	doc.LoadFile();

	// Get the root element, ie <Tiles>...</Tiles>
	TiXmlElement* root = doc.FirstChildElement();

	// Loop through all different tiles and load them into the tile data map
	for(TiXmlElement* tile = root->FirstChildElement("Tile"); tile != NULL; tile = tile->NextSiblingElement("Tile"))
	{
		// Data to be added
		TileData* data = new TileData();

		// Get the tiles attributes
		data->name = tile->Attribute("name");
		data->texture = gGraphics->loadTexture(tile->Attribute("texture"));
		data->textureSource = tile->Attribute("texture");
		data->friction = atof(tile->Attribute("friction"));
		string obstacle = tile->Attribute("obstacle");

		data->obstacle = false;
		if(obstacle == "true")
			data->obstacle = true;

		// Insert the new data into the map
		mDataMap.insert(TilePair(data->name, data));
	}
}

TileData* TileHandler::getData(string name)
{
	// Search for tile data with the key 'name' and return it
	TileIter iter = mDataMap.find(name);
	return iter->second;
}

map<string, TileData*> TileHandler::getDataMap()
{
	// Returns the whole data map
	return mDataMap;
}