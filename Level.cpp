#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h"
#include <fstream>
#include "Level.h"
#include "Player.h"
#include "Object.h"
#include "Collision.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "Tile.h"
#include "TileHandler.h"
#include "Enemy.h"
#include "Structure.h"
#include "Loot.h"
#include "Projectile.h"

Level::Level()
{
	
}

Level::~Level()
{
	// Save the level if in editor
	if(isInEditor())
		saveToFile("level.xml");

	// Delete the tile handler
	delete mTileHandler;

	// Delete all objects
	for(int i = 0; i < mObjectList.size(); i++)
		delete mObjectList[i];

	// Clear the object list
	mObjectList.clear();

	// Delete all tiles
	for(int i = 0; i < mTileList.size(); i++)
		delete mTileList[i];

	// Clear the tile list
	mTileList.clear();
}

void Level::init()
{
	// Create the tilehandler and load the different tiles from a XML file
	mTileHandler = new TileHandler();
	mTileHandler->loadTiles("tiles.xml");
	mOffset = Vector(0, 0);

	mTileWidth = 40;
	mTileHeight = 40;

	// Load tile placements from a textfile
	loadFromFile("level.xml");

	// A player has to be added manually
	mPlayer = NULL;
}

void Level::update(float dt)
{
	MTV mtv;
	mtv.collision = false;
	int size = mObjectList.size();

	for(int i = 0; i < size; i++)
	{
		if(mObjectList[i]->getAlive()) {
			if(!isInEditor()) 
				mObjectList[i]->update(dt);
		}
		else	{
			removeObject(mObjectList[i]);
			size--;
			i--;
			continue;
		}

		for(int j = 0; j < mObjectList.size(); j++)
		{
			if(mObjectList[i]->getType() == PROJECTILE && mObjectList[j]->getType() == PROJECTILE)
				continue;

			// Get collision information
			//if (usesCollision() == true)
			mtv = checkCollision(mObjectList[i]->getPolygon(), mObjectList[j]->getPolygon());

			// If there's a collision
			if(mtv.collision == true && mObjectList[i]->getID() != mObjectList[j]->getID())	
			{
				//If Type == projectile
				if((mObjectList[i]->getType() == PROJECTILE || mObjectList[j]->getType() == PROJECTILE))
				{
					// No shooter collision
					if (!(mObjectList[j]->getOwnerId() == mObjectList[i]->getID() || mObjectList[i]->getOwnerId() == mObjectList[j]->getID()))
					{
						// Find out which one is the projectile
						if(mObjectList[i]->getType() != PROJECTILE) 
						{
							// Kill the projectile
							mObjectList[j]->setAlive(false);
							if(mObjectList[i]->getType() == ENEMY)
								mObjectList[i]->handleCollision(mObjectList[j], &mtv);
						}
						else 
						{
							// Kill the projectile
							mObjectList[i]->setAlive(false);
							if(mObjectList[j]->getType() == ENEMY)
								mObjectList[j]->handleCollision(mObjectList[i], &mtv);
						}
					}
				}
				else
				{
					// Move the object out of collision
					if(mObjectList[i]->getType() == PLAYER) 
						moveObjects(mtv.pushX, mtv.pushY);
					else
						mObjectList[i]->move(-mtv.pushX, -mtv.pushY);

					mObjectList[i]->handleCollision(mObjectList[j], &mtv);
				}
			}

		}
	}

	// Check players collision with tiles
	for(int i = 0; i < mObjectList.size(); i++)
	{
		Object* object = mObjectList[i];
		for(int j = 0; j < mTileList.size(); j++)
		{
			Tile* tile = mTileList[j];
			// Players bounding box
			Rect boundingBox = object->getBoundingBox();
			Rect tileRect;
			tileRect.left = tile->getPosition().x - mTileWidth/2 + mOffset.x;
			tileRect.right = tile->getPosition().x + mTileWidth/2 + mOffset.x;
			tileRect.top = tile->getPosition().y - mTileHeight/2 + mOffset.y;
			tileRect.bottom = tile->getPosition().y + mTileHeight/2 + mOffset.y;

			// Check bounding boxes
			if(boundingBox.left > tileRect.right || boundingBox.right < tileRect.left || boundingBox.top > tileRect.bottom || boundingBox.bottom < tileRect.top)
				continue;
			// Collidable?
			if(!mTileHandler->getData(tile->getName())->obstacle)
				continue;

			// Temporary test object, with position and dimensions of the tile
			Object* tileObject = new Object(mOffset.x + tile->getPosition().x, mOffset.y + tile->getPosition().y, mTileWidth, mTileHeight, STRUCTURE, "Data\\imgs\\standard_box.bmp");

			// Get information about the collision
			mtv = checkCollision(object->getPolygon(), tileObject->getPolygon());
						
			// Move the player out of the tile if there's a collision
			if(mtv.collision)	{
				if(object->getType() == PLAYER) 
					moveObjects(mtv.pushX, mtv.pushY);
				else
					object->move(-mtv.pushX, -mtv.pushY);

				object->handleCollision(tileObject, &mtv);
			}
			delete tileObject;
		}
	}
}

void Level::draw()
{
	// Loop through and draw all tiles
	for(int i = 0; i < mTileList.size(); i++) {
		if(mTileList[i]->getName() != "Bkgd")	// :TODO: To consuming
			gGraphics->drawTexture(mTileList[i]->getData()->texture, mOffset.x + mTileList[i]->getPosition().x, mOffset.y + mTileList[i]->getPosition().y, mTileWidth, mTileHeight);
	}

	// Draw the different layers
	for(int i = 0; i < mObjectList.size(); i++)
		if(mObjectList[i]->getLayer()==BOTTOM) mObjectList[i]->draw();
	for(int i = 0; i < mObjectList.size(); i++)
		if(mObjectList[i]->getLayer()==MIDDLE) mObjectList[i]->draw();
	// NOTE: HACK!
	//mPlayer->draw();
	for(int i = 0; i < mObjectList.size(); i++)
		if(mObjectList[i]->getLayer()==TOP) mObjectList[i]->draw();

	// Print how many objects there is on the level
	char buffer[256];
	sprintf(buffer, "objects: %i", mObjectList.size());
	gGraphics->drawText(buffer, 10, 300);

	//sprintf(buffer, "offset: %f", mOffset.x*-1);
	//gGraphics->drawText(buffer, 300, 300, CUSTOM, 20, 0xffff0000);
}

void Level::addObject(Object* object)
{
	// Make sure each object gets a unique ID, it's with the ID we can poll which object we want
	static int nextID = 0;
	object->setID(nextID);
	object->setLevel(this);
	nextID++;

	mObjectList.push_back(object);
}

void Level::addPlayer(Player* player)
{
	// Both set mPlayer to the player and add it to the object list
	// mPlayer is used for simplicity
	mPlayer = player;
	addObject(mPlayer);
}

void Level::removeObject(Object* object)
{
	// Loop through all objects and find out which one to delete
	int i = 0;
	std::vector<Object*>::iterator itr =  mObjectList.begin();
	while(itr != mObjectList.end() && i < mObjectList.size())
	{
		if(mObjectList[i]->getID() == object->getID())
		{
			delete mObjectList[i];		// viktigt att deleta innan!
			mObjectList[i] = NULL;
			itr = mObjectList.erase(itr);	
			break;
		}
		else	{
			itr++;
			i++;
		}
	}
}

void Level::removeObjectAt(float x, float y)
{
	Vector pos = Vector(x, y);
	for(int i = 0; i < mObjectList.size(); i++) 
	{
		if(gMath->pointInsideRect(pos, mObjectList[i]->getRect())) {
			removeObject(mObjectList[i]);
			break;
		}
	}
}

Object* Level::getObjectAt(Vector pos)
{
	for(int i = 0; i < mObjectList.size(); i++) 
	{
		if(gMath->pointInsideRect(pos, mObjectList[i]->getRect())) {
			return mObjectList[i];
		}
	}

	return NULL;
}

void Level::moveObjects(float dx, float dy)
{
	for(int i = 0; i < mObjectList.size(); i++)	{
		if(mObjectList[i]->getType() !=  PLAYER)
			mObjectList[i]->move(dx, dy);
	}

	mOffset += Vector(dx, dy);

	/*Vector newOffset = (mOffset + Vector(dx, dy)) *-1;

	if(!isInEditor())
	{
		if((newOffset.x >= 0 && mPlayer->getPos().x > mWidth/2) )// || (newOffset.x <= mWidth-400 && mPlayer->getPos().x < mWidth/2))
		//if(newOffset.x >= 0 && newOffset.y >= 0 && newOffset.x <= mWidth-400 && newOffset.y <= mHeight-300)
		{
			for(int i = 0; i < mObjectList.size(); i++)	{
				if(mObjectList[i]->getType() !=  PLAYER)
					mObjectList[i]->move(dx, dy);
			}

			mOffset += Vector(dx, dy);
		}
		else 
			mPlayer->move(-dx, -dy);	
	}*/
}

void Level::saveToFile(string file)
{
	// The XML document
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");

	// Add the version info
    doc.LinkEndChild(decl);
    
	// Create the different elements
    TiXmlElement* root = new TiXmlElement("Level");
	TiXmlElement* size = new TiXmlElement("Size");
	TiXmlElement* objects = new TiXmlElement("Objects");
	TiXmlElement* tiles = new TiXmlElement("Tiles");
    
	// Add elements
	root->LinkEndChild(size);
	root->LinkEndChild(objects);
	root->LinkEndChild(tiles);
	doc.LinkEndChild(root);

	size->SetAttribute("width", mWidth);
	size->SetAttribute("height", mHeight);

	// Loop through objects and save them to the file
	for(int i = 0; i < mObjectList.size(); i++)
	{
		// Don't save the player
		if(mObjectList[i]->getType() == PLAYER)
			continue;

		TiXmlElement* object;
		string name = "Object";
		if(mObjectList[i]->getType() == ENEMY)
			name = "Enemy";

		// Set attributes
		object = new TiXmlElement(name.c_str());
		object->SetAttribute("type", mObjectList[i]->getType());
		object->SetAttribute("x", mObjectList[i]->getPos().x - mOffset.x);
		object->SetAttribute("y", mObjectList[i]->getPos().y - mOffset.y);
		
		if(mObjectList[i]->getType() == ENEMY) {
			Enemy* enemy = dynamic_cast<Enemy*>(mObjectList[i]);
			object->SetAttribute("class", enemy->getClass()->name.c_str());
		}

		object->SetAttribute("texture", mObjectList[i]->getTextureSource().c_str());

		// Add to the XML element
		objects->LinkEndChild(object);
	}

	// Loop through tiles and save them to the file
	for(int i = 0; i < mTileList.size(); i++)
	{
		TiXmlElement* tile = new TiXmlElement("Tile");
		tile->SetAttribute("name", mTileList[i]->getName().c_str());
		tile->SetAttribute("x", mTileList[i]->getPosition().x);
		tile->SetAttribute("y", mTileList[i]->getPosition().y);
		tiles->LinkEndChild(tile);
	}

	// Save to file
	doc.SaveFile(file.c_str());
}
	
void Level::loadFromFile(string file)
{
	// The XML document
	TiXmlDocument doc(file.c_str());
	doc.LoadFile();

	// Get the different elements
	TiXmlElement* root = doc.FirstChildElement();
	TiXmlElement* sizeElement = root->FirstChildElement();
	TiXmlElement* objectElement = sizeElement->NextSiblingElement();
	TiXmlElement* tileElement = objectElement->NextSiblingElement();

	mWidth = atoi(sizeElement->Attribute("width"));
	mHeight = atoi(sizeElement->Attribute("height"));

	// Loop through the objects and add them to the object list
	for(TiXmlElement* object = objectElement->FirstChildElement(); object != NULL; object = object->NextSiblingElement())
	{
		// Get attributes
		int type = atoi(object->Attribute("type"));
		int x = atoi(object->Attribute("x"));
		int y = atoi(object->Attribute("y"));
		string texture = object->Attribute("texture");

		// Switch type and add the according object to the level
		if(type == ObjectType::STRUCTURE) {
			Structure* structure = new Structure(x, y, 100, 100, texture);
			addObject(structure);
		}
		else if(type == ObjectType::ENEMY) {
			Enemy* enemy = new Enemy(x, y, gEnemies->getData(object->Attribute("class")));
			addObject(enemy);
		}
		// TODO: Add for different objects as well
	}

	// Loop through all tiles and load add them to the tile list
	for(TiXmlElement* tile = tileElement->FirstChildElement("Tile"); tile != NULL; tile = tile->NextSiblingElement("Tile"))
	{
		string name = tile->Attribute("name");
		int x = atoi(tile->Attribute("x"));
		int y = atoi(tile->Attribute("y"));
		addTile(x, y, name);
	}
}

//! Adds a tile to the level.
/**
@note X and Y are in game coordinates. This function calculates the proper position in the tile grid.
*/
void Level::addTile(float x, float y, string name)
{
	if(y < 0)
		int asda =1;

	float oldX = x;
	float oldY = y;

	// Has to remove one tile dimension, since the integer division rounding
	if(x < 0) x -= getTileWidth();
	if(y < 0) y -= getTileHeight();

	// Transform to proper position in the grid
	x = (int)x / getTileWidth();
	y = (int)y / getTileHeight();

	x = x * getTileWidth() + getTileWidth()/2;
	y = y * getTileHeight() + getTileHeight()/2;

	if(getTile(x, y) != "#NOVALUE")
		removeTile(oldX, oldY);
	
	Tile* tile = new Tile(x, y, name);
	tile->setData(mTileHandler->getData(name));
	mTileList.push_back(tile);
}

void Level::removeTile(float x, float y)
{
	if(x < 0) x -= getTileWidth();
	if(y < 0) y -= getTileHeight();

	x = (int)x / getTileWidth();
	y = (int)y / getTileHeight();

	x = x * getTileWidth() + getTileWidth()/2;
	y = y * getTileHeight() + getTileHeight()/2;

	int i = 0;
	auto iter = mTileList.begin();
	while(iter != mTileList.end())
	{
		if((*iter)->getPosition() == Vector(x, y)) {
			delete mTileList[i];
			mTileList[i] = NULL;
			iter = mTileList.erase(iter);
			break;
		}
		else {
			iter++;
			i++;
		}
	}
}

string Level::getTile(int x, int y)
{
	for(int i = 0; i < mTileList.size(); i++) 
	{
		if(mTileList[i]->getPosition() == Vector(x, y)) 
			return mTileList[i]->getName();
	}

	return "#NOVALUE";
}

void Level::addProjectile(Object* shooter, Vector target, int spread) 
{
	float v = gMath->calculateAngle(shooter->getPos(), target);
	ObjectData data;
	data.width = 20;
	data.height = 7;
	data.drawLayer = MIDDLE;
	data.textureSource = "Data\\imgs\\projectile.bmp";
	Object* object = new Projectile(shooter->getPos().x, shooter->getPos().y, data.width, data.height, 10, data.textureSource);
	object->setLayer(data.drawLayer);
	object->setOwnerId(shooter->getID());
	object->setType(PROJECTILE);
	object->setRotation(v + (rand() % spread - spread/2)/40.0f);
	addObject(object);
}

Vector Level::getOffset()
{
	return mOffset;
}

int Level::getTileWidth()
{
	return mTileWidth;
}

int Level::getTileHeight()
{
	return mTileHeight;
}

TileHandler* Level::getTileHandler()
{
	return mTileHandler;
}

Player* Level::getPlayer()
{
	return mPlayer;
}