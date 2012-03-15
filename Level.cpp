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
#include "Scrap.h"
#include "Collision.h"
#include "Region.h"
#include "Gold.h"
#include "EnergyPotion.h"
#include "HealthPotion.h"

Level::Level()
{
	
}

Level::~Level()
{
	// Save the level if in editor
	if(isInEditor())
		saveToFile(mSource);

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
	mTileHandler->loadTiles("Data\\tiles.xml");
	mOffset = Vector(0, 0);
	mCameraOffset = mOffset;
	mChangeState = false;
	mEditorMode = false;

	mTileWidth = 40;
	mTileHeight = 40;

	// Load tile placements from a textfile
	//loadFromFile("Data\\levels\\campaign.xml");

	// A player has to be added manually
	mPlayer = NULL;
}

void Level::update(float dt)
{
	MTV mtv;
	mtv.collision = false;
	int size = mObjectList.size();

	for(int i = 0; i < mObjectList.size(); i++)
	{
		Object* objectA = mObjectList[i];
		if(objectA->getAlive()) {
			if(!isInEditor()) {
				objectA->update(dt);
				if(mChangeState)
					return;
			}
		}
		else	{
			if(objectA->getType() == ENEMY)
				getPlayer()->addExperience(dynamic_cast<Enemy*>(objectA)->getExperience());

			removeObject(objectA);
			size--;
			i--;
			continue;
		}

		for(int j = i+1; j < mObjectList.size(); j++)
		{
			Object* objectB = mObjectList[j];

			if(objectA->getType() == PROJECTILE && objectB->getType() == PROJECTILE)
				continue;

			if(!objectA->getColides() || !objectB->getColides())
				continue;

			if(objectA->getID() == objectB->getOwnerId() || objectB->getID() == objectA->getOwnerId())
				continue;

			mtv = checkCollision(objectA->getPolygon(), objectB->getPolygon(), objectA->detailedCollision() && objectB->detailedCollision());

			// If there's a collision
			if(mtv.collision)
			{
				objectA->handleCollision(objectB,&mtv);
				objectB->handleCollision(objectA,&mtv);

				if(objectA->getType() == PLAYER && objectB->getType() != PROJECTILE)
					moveObjects(mtv.pushX, mtv.pushY);
				else if(objectA->getType() == ENEMY)
					objectA->move(-mtv.pushX, -mtv.pushY);
				else if(objectB->getType() == ENEMY)
					objectB->move(mtv.pushX, mtv.pushY);
				else if(objectB->getType() == PLAYER)
					moveObjects(-mtv.pushX, -mtv.pushY);
			}
		}

		for(int j = 0; j < mTileList.size(); j++)
		{
			Tile* tile = mTileList[j];

			// Collidable?
			if(!tile->getData()->obstacle)
				continue;
			if(!objectA->getColides())
				continue;
			gScrap->tileObject->setPos(Vector(mOffset.x + tile->getPosition().x, mOffset.y + tile->getPosition().y));
			// Players bounding box
			Rect boundingBox = objectA->getBoundingBox();
			gScrap->rect->setPos(Vector(tile->getPosition().x + mOffset.x, tile->getPosition().y + mOffset.y));

			// Check bounding boxes
			if(boundingBox.left > gScrap->rect->right || boundingBox.right < gScrap->rect->left || boundingBox.top > gScrap->rect->bottom || boundingBox.bottom < gScrap->rect->top)
				continue;

			// Get information about the collision
			mtv = checkCollision(objectA->getPolygon(), gScrap->tileObject->getPolygon());

			// Move the player out of the tile if there's a collision
			if(mtv.collision)	{
				if(objectA->getType() == PLAYER) 
					moveObjects(mtv.pushX, mtv.pushY);
				else 
				{
					objectA->move(-mtv.pushX, -mtv.pushY);
					objectA->handleCollision(gScrap->tileObject, &mtv);
				}
			}
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
	if(!isInEditor())
		mPlayer->draw();

	for(int i = 0; i < mObjectList.size(); i++)
		if(mObjectList[i]->getLayer()==TOP) mObjectList[i]->draw();

	if(!isInEditor())
		mPlayer->drawInventory();
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
	mCameraOffset.x = mPlayer->getPos().x -  SCREEN_WIDTH/2;
	mCameraOffset.y = SCREEN_HEIGHT/2 - mPlayer->getPos().y;
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
	mCameraOffset += Vector(-dx, -dy);
	Vector newOffset = (mOffset + Vector(dx, dy)) *-1;

	// NOTE: 400 is screenWidth/2 and 300 is screenHeight/2

	// Horizontal camera movement
	if(!isInEditor())
	{
		if(mCameraOffset.x < 0 || mCameraOffset.x + mWidth/2 > mWidth-1024/2) {
			if(mPlayer != NULL) mPlayer->move(-dx, 0);
			dx = 0;
		}

		// Vertical camera movement
		if(mCameraOffset.y < 0 || mCameraOffset.y + mHeight/2 > mHeight-SCREEN_HEIGHT/2) {
			if(mPlayer != NULL) mPlayer->move(0, -dy);
			dy = 0;
		}

		for(int i = 0; i < mObjectList.size(); i++)
			if(mObjectList[i]->getType() !=  PLAYER)
				mObjectList[i]->move(dx, dy);
	}
	else
	{
		// +200 due to the editor gui
		if(newOffset.x < 0 || newOffset.x + mWidth/2 > mWidth-SCREEN_WIDTH/2+200)
			dx = 0;

		if(newOffset.y < 0 || newOffset.y + mHeight/2 > mHeight-SCREEN_HEIGHT/2) 
			dy = 0;

		for(int i = 0; i < mObjectList.size(); i++)	
			mObjectList[i]->move(dx, dy);
	}

	mOffset += Vector(dx, dy);
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
		else if(mObjectList[i]->getType() == LOOT) 
			name = "Loot";
		
		// Set attributes
		object = new TiXmlElement(name.c_str());
		object->SetAttribute("type", mObjectList[i]->getType());
		object->SetAttribute("x", mObjectList[i]->getPos().x - mOffset.x);
		object->SetAttribute("y", mObjectList[i]->getPos().y - mOffset.y);
		object->SetAttribute("rotation", mObjectList[i]->getRotation()*1000);
		mObjectList[i]->setRotation(0.0f);
		object->SetAttribute("w", mObjectList[i]->getBoundingBox().getWidth());
		object->SetAttribute("h", mObjectList[i]->getBoundingBox().getHeight());
		object->SetAttribute("collidable", mObjectList[i]->getColides() ? 1 : 0);
		
		
		if(mObjectList[i]->getType() == ENEMY) {
			Enemy* enemy = dynamic_cast<Enemy*>(mObjectList[i]);
			object->SetAttribute("class", enemy->getClass()->name.c_str());
		}

		if(mObjectList[i]->getType() == LOOT) {
			Loot* loot = dynamic_cast<Loot*>(mObjectList[i]);
			object->SetAttribute("name", loot->getName().c_str());
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
	mSource = file;

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
		int w = atoi(object->Attribute("w"));
		int h = atoi(object->Attribute("h"));
		bool collides = atoi(object->Attribute("collidable")) == 1 ? true : false;
		float rotation = atof(object->Attribute("rotation")) / 1000;
		string texture = object->Attribute("texture");

		// Switch type and add the according object to the level
		if(type == ObjectType::STRUCTURE) {
			Structure* structure = new Structure(x, y, w, h, texture);
			structure->setCollidable(collides);
			structure->setRotation(rotation);
			addObject(structure);
		}
		else if(type == ObjectType::ENEMY) {
			Enemy* enemy = new Enemy(x, y, gEnemies->getData(object->Attribute("class")));
			addObject(enemy);
		}
		else if(type == ObjectType::REGION) {
			Region* region = new Region(Rect(x,y,w,h,0));
			region->initTrigger(region);
			addObject(region);
		}
		else if(type == ObjectType::LOOT) {
			string name = object->Attribute("name");
			if(name == "Health Potion") {
				HealthPotion* hp = new HealthPotion(x, y);
				hp->setLifetime(0);
				addObject(hp);
			}
			else if(name == "Energy Potion") {
				EnergyPotion* ep = new EnergyPotion(x, y);
				ep->setLifetime(0);
				addObject(ep);
			}
			else if(name == "Gold") {
				Gold* gold = new Gold(x, y);
				gold->setLifetime(0);
				addObject(gold);
			}
			else { 
				Loot* loot = new Loot(name, x, y);
				loot->setLifetime(0);
				addObject(loot);
			}
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

void Level::addProjectile(Object* shooter, Vector pos, Vector target, ProjectileData pData) 
{
	float v = gMath->calculateAngle(shooter->getPos(), target);
	ObjectData data;
	data.width = pData.width;
	data.height = pData.height;
	data.drawLayer = MIDDLE;
	data.textureSource = pData.texturePath;
	Projectile* object = new Projectile(pos.x, pos.y, data.width, data.height, pData.speed, data.textureSource);
	object->setOffset(object->getPos() - shooter->getPos());
	object->setOwner(mPlayer);
	object->setFollowingPlayer(pData.followPlayer);
	object->setLifetime(pData.lifetime);
	object->setLayer(data.drawLayer);
	object->setOwnerId(shooter->getID());
	object->setType(PROJECTILE);
	object->setRotation(v + (rand() % pData.spread - pData.spread/2)/40.0f);
	object->setMaxDistance(pData.range);
	object->setDamage(pData.damage);
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

int Level::getWidth()
{
	return mWidth;
}

int Level::getHeight()
{
	return mHeight;
}