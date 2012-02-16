#include "Editor.h"
#include "Structure.h"
#include "Input.h"
#include "wButton.h"
#include "wPanel.h"
#include "wDropDown.h"
#include "enums.h"
#include "wContainer.h"
#include "Object.h"
#include <time.h>
#include "Tile.h"
#include "TileHandler.h"
#include "Level.h"
#include "Enemy.h"
#include "ItemHandler.h"
#include "Loot.h"
#include "Region.h"
#include "Math.h"

Editor::Editor(std::string levelName)
{
	//// Create the level
	//mLevel = new Level();

	//mLevel->init();

	//mWindowHandler = new WindowHandler();

	//mButton1 = new wButton(600, 400, 100, 50, "press me!", WID_TESTBUTTON1);
	//mButton1->connect(&Editor::messageHandler,this);
	//mWindowHandler->addWindow(mButton1);

	//wPanel* panel = new wPanel(700, 300, 200, 600, WID_PANEL1);
	////mWindowHandler->addWindow(panel);

}

Editor::Editor()
{
	// Create the level
	mLevel = new Level();
	mLevel->init();
	mLevel->setInEditor(true);

	// Set the default active tile type
	mActiveObject = ActiveObject("Grass", TILE);

	// Init the GUI stuff
	mWindowHandler = new WindowHandler();

	wPanel* panel = new wPanel(1024-105, 300, 210, 900, WID_PANEL1, "Data\\imgs\\panel.bmp");
	mWindowHandler->addWindow(panel);

	mDropDown = new wDropDown(1024-105, 20, 210, 20, WID_OBJECTTYPE);
	mDropDown->add("Tiles");
	mDropDown->add("Props");
	mDropDown->add("Enemies");
	mDropDown->add("Loot");
	mDropDown->add("SFX");
	mDropDown->add("Regions");
	mDropDown->setZ(2);
	mDropDown->connect(&Editor::messageHandler, this);
	mWindowHandler->addWindow(mDropDown);

	mButtonContainer = new wContainer(1024-105, 230, 210, 300, WID_BUTTONCONTAINER);
	mButtonContainer->setItemDimensions(40, 40);
	mButtonContainer->setPadding(5, 5);
	mWindowHandler->addWindow(mButtonContainer);
}

Editor::~Editor()
{
	delete mLevel;
	delete mWindowHandler;
}

void Editor::update(float dt)
{
	mLevel->update(dt);
	mWindowHandler->update(dt);

	Vector pos = gInput->mousePosition();
	if((gInput->keyDown(VK_LBUTTON) && mActiveObject.type == TILE) || (gInput->keyPressed(VK_LBUTTON) && mActiveObject.type != TILE) && pos.x < 595)
	{
		// TODO: Fix correct placement
		if(mActiveObject.type == ENEMY) {
			Enemy* enemy = new Enemy(pos.x, pos.y, gEnemies->getData(mActiveObject.name));
			mLevel->addObject(enemy);
		}
		else if(mActiveObject.type == TILE) {
			mLevel->addTile(pos.x - mLevel->getOffset().x, pos.y - mLevel->getOffset().y, mActiveObject.name);
		}
		else if(mActiveObject.type == LOOT) {
			mLevel->addObject(new Loot(mActiveObject.name, pos.x, pos.y));
		}
		else if(mActiveObject.type == REGION) {
			mClickedPos = Vector(pos.x - mLevel->getOffset().x, pos.y - mLevel->getOffset().y);
		}
	}
	if(gInput->keyDown(VK_RBUTTON))
	{
		Vector pos = gInput->mousePosition();

		if(mActiveObject.type == TILE)
			mLevel->removeTile(pos.x - mLevel->getOffset().x, pos.y - mLevel->getOffset().y);
		else {
			mLevel->removeObjectAt(pos.x, pos.y);
		}
	}
	if(gInput->keyReleased(VK_LBUTTON) && mActiveObject.type == REGION)
	{
		Vector tP = pos;
		pos.x-=mLevel->getOffset().x;
		pos.y-=mLevel->getOffset().y;
		int h = gMath->delta(mClickedPos.y, pos.y);
		int w = gMath->delta(mClickedPos.x, pos.x);
		mLevel->addObject(new Region(mClickedPos.x, mClickedPos.y, w, h));
	}

	if(gInput->keyDown('W'))	
		mLevel->moveObjects(0, 5);
	else if(gInput->keyDown('S'))	
		mLevel->moveObjects(0, -5);
	if(gInput->keyDown('A'))	
		mLevel->moveObjects(5, 0);
	else if(gInput->keyDown('D'))	
		mLevel->moveObjects(-5, 0);
}

void Editor::draw()
{
	// Draw game content
	mLevel->draw();
	mWindowHandler->draw();
}

bool Editor::messageHandler(wId id, wMessage msg)
{
	if(id == WID_OBJECTTYPE)
	{
		mButtonContainer->clearChildren(mWindowHandler);
		if(msg.getString()  == "Tiles")	
		{
			mActiveObject.type = TILE;
			// Create the tile buttons
			map<string, TileData*> dataMap = mLevel->getTileHandler()->getDataMap();
			mActiveObject.name = (*dataMap.begin()).second->name;

			for(TileIter iter = dataMap.begin(); iter != dataMap.end(); iter++)
			{
				wButton* button = new wButton(10, 10, 40, 40, iter->second->name, WID_TILE_BUTTON, iter->second->textureSource);
				button->connect(&Editor::messageHandler, this);
				mWindowHandler->addWindow(button);
				mButtonContainer->arrangeObject(button);
			}
		}
		else if(msg.getString() == "Props")
		{
			mActiveObject.type = PROP;
		}
		else if(msg.getString() == "Enemies")	{
			mActiveObject.type = ENEMY;
			
			// Create the enemy buttons
			map<string, EnemyData*> dataMap = gEnemies->getDataMap();

			mActiveObject.name = (*dataMap.begin()).second->name;
			for(std::map<string, EnemyData*>::iterator iter = dataMap.begin(); iter != dataMap.end(); iter++)
			{
				wButton* button = new wButton(10, 10, 40, 40, iter->second->name, WID_OBJECT_BUTTON, iter->second->textureSource);
				button->connect(&Editor::messageHandler, this);
				mWindowHandler->addWindow(button);
				mButtonContainer->arrangeObject(button);
			}
		}
		else if(msg.getString() == "Loot")	{
			mActiveObject.type = LOOT;
			map<string, ItemData> attributeMap = gItemHandler->getDataMap();
			 
			for(auto iter = attributeMap.begin(); iter != attributeMap.end(); iter++)
			{
				wButton* button = new wButton(10, 10, 40, 40, iter->second.name, WID_OBJECT_BUTTON, iter->second.textureSource);
				button->connect(&Editor::messageHandler, this);
				mWindowHandler->addWindow(button);
				mButtonContainer->arrangeObject(button);
			}
		}
		else if(msg.getString() == "SFX")	{
			mActiveObject.type = SFX;
		}
		else if(msg.getString() == "Regions")	{
			mActiveObject.type = REGION;
		}
	}
	else if(id == WID_TILE_BUTTON || WID_OBJECT_BUTTON)
	{
		mActiveObject.name = msg.getString();
	}

	return true;
}