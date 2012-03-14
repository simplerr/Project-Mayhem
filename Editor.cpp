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
#include "Scrap.h"
#include "Math.h"
#include "Graphics.h"
#include "wLabel.h"
#include "Game.h"
#include "EnergyPotion.h"
#include "HealthPotion.h"
#include "Gold.h"
#include "ObjectHandler.h"

#define IDC_INPUT_BOX 101
#define IDC_ENTER_PRESSED 102

// The default edit control procedure.
WNDPROC DefEditProc;

//! Subclassed msg proc for the input box.
LRESULT inputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	bool enter = false;
    switch (uMsg)
    {
	case WM_CHAR:
		// Enter was pressed.
		if(wParam == VK_RETURN)
			enter = true;
	default:
		// Handles all default actions.
		result =  CallWindowProc(DefEditProc, hwnd, uMsg, wParam, lParam);
    }

	// Has to be here since case WM_CHAR is before the default procedure. 
	// Otherwise the caret positions gets set but then changed when the ENTER msg is proccessed.
	if(enter)
	{
		// Send the message so Chat can catch it.
		SendMessage(gGame->getMainWnd(), IDC_ENTER_PRESSED, 0, 0);
	}

	return result;
}

Editor::Editor(std::string levelName)
{

}

Editor::Editor()
{
	// Create the level
	mLevel = new Level();
	mLevel->init();
	mLevel->setInEditor(true);
	mLevel->loadFromFile("Data\\levels\\custom.xml");

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

	mTileLabel1 = NULL;
	mTileLabel2 = NULL;
	mSelectedRegion = NULL;
	mMovingObject = NULL;
}

Editor::~Editor()
{
	DestroyWindow(mhInputBox);
	delete mLevel;
	delete mWindowHandler;
}

void Editor::update(float dt)
{
	mLevel->update(dt);
	mWindowHandler->update(dt);

	Vector pos = gInput->mousePosition();
	if(gInput->keyPressed(VK_LBUTTON))
	{
		Object* object = mLevel->getObjectAt(pos);

		if(object != NULL) {
			mMovingObject = object;
			mMovingOffset = mMovingObject->getPos() - pos;
		}
		else
			mMovingObject = NULL;
	}

	if(gInput->mouseDz() != 0 && mMovingObject != NULL)
		mMovingObject->rotate(gInput->mouseDz()/1500.0f);

	if(gInput->keyDown(VK_LBUTTON) && mMovingObject != NULL)
		mMovingObject->setPos(pos + mMovingOffset);

	if(((gInput->keyDown(VK_LBUTTON) && mActiveObject.type == TILE) || (gInput->keyPressed(VK_LBUTTON) && mActiveObject.type != TILE)) && (pos.x < 824))
	{
		// TODO: Fix correct placement
		if(mLevel->getObjectAt(pos) == NULL)
		{
			if(mActiveObject.type == ENEMY) {
				Enemy* enemy = new Enemy(pos.x, pos.y, gEnemies->getData(mActiveObject.name));
				mLevel->addObject(enemy);
			}
			else if(mActiveObject.type == TILE) {
				mLevel->addTile(pos.x - mLevel->getOffset().x, pos.y - mLevel->getOffset().y, mActiveObject.name);
			}
			else if(mActiveObject.type == LOOT) {
				if(mActiveObject.name == "Health Potion")
					mLevel->addObject(new HealthPotion(pos.x, pos.y));
				else if(mActiveObject.name == "Energy Potion")
					mLevel->addObject(new EnergyPotion(pos.x, pos.y));
				else if(mActiveObject.name == "Gold")
					mLevel->addObject(new Gold(pos.x, pos.y));
				else
					mLevel->addObject(new Loot(mActiveObject.name, pos.x, pos.y));
			}
			else if(mActiveObject.type == REGION) {
				mSelectedRegion = dynamic_cast<Region*>(mLevel->getObjectAt(pos));
				if(mSelectedRegion == NULL) 
				{
					mClickedPos = Vector(pos.x - mLevel->getOffset().x, pos.y - mLevel->getOffset().y);
					//mClickedPos = Vector(pos.x - mLevel->getOffset().x, pos.y - mLevel->getOffset().y); //MINUS
					mClickedPos = Vector(pos.x, pos.y);
					gScrap->editingRegion=true;
					gScrap->editorRect->setRect(mClickedPos.x, mClickedPos.x+1, mClickedPos.y, mClickedPos.y+1);
				}
				else 
				{
					SetWindowText(mhInputBox, mSelectedRegion->getTrigger()->getTriggerAsString().c_str());
				}
			}
			else if(mActiveObject.type == PROP) {
				auto data = gObjectHandler->getData(mActiveObject.name);
				Structure* structure = new Structure(pos.x, pos.y, data.width, data.height, data.textureSource);
				structure->setCollidable(data.collides);
				mLevel->addObject(structure);
			}
		}
	}
	if(gScrap->editingRegion) {
		float t;
		gScrap->editorRect->right = pos.x;
		gScrap->editorRect->bottom = pos.y;
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
	if(gInput->keyReleased(VK_LBUTTON) && mActiveObject.type == REGION && (pos.x < 824) && gScrap->editingRegion)
	{
		float t;
		if(gScrap->editorRect->bottom < gScrap->editorRect->top) {
			t = gScrap->editorRect->bottom;
			gScrap->editorRect->bottom = gScrap->editorRect->top;
			gScrap->editorRect->top = t;
		}
		if(gScrap->editorRect->right < gScrap->editorRect->left) {
			t = gScrap->editorRect->right;
			gScrap->editorRect->right = gScrap->editorRect->left;
			gScrap->editorRect->left = t;
		}
		Region * r = new Region(*gScrap->editorRect);
		r->initTrigger(r);
		mLevel->addObject(r);
		gScrap->editingRegion=false;
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
	if(gScrap->editingRegion) 
		gGraphics->drawRect(*gScrap->editorRect, D3DCOLOR_ARGB(150,20,200,20));
	mWindowHandler->draw();
}

bool Editor::messageHandler(wId id, wMessage msg)
{
	if(id == WID_OBJECTTYPE)
	{
		DestroyWindow(mhInputBox);
		mWindowHandler->removeWindow(mTileLabel1);
		mWindowHandler->removeWindow(mTileLabel2);
		mButtonContainer->clearChildren(mWindowHandler);
		if(msg.getString()  == "Tiles")	
		{
			mActiveObject.type = TILE;
			// Create the tile buttons
			map<string, TileData*> dataMap = mLevel->getTileHandler()->getDataMap();
			mActiveObject.name = (*dataMap.begin()).second->name;

			mTileLabel1 = new wLabel(820, 60, 0, 0, WID_STANDARD, "Walls");
			mWindowHandler->addWindow(mTileLabel1);

			mTileLabel2 = new wLabel(820, 260, 0, 0, WID_STANDARD, "Ground");
			mWindowHandler->addWindow(mTileLabel2);

			for(TileIter iter = dataMap.begin(); iter != dataMap.end(); iter++)
			{
				if(!iter->second->obstacle)
					continue;

				wButton* button = new wButton(10, 10, 40, 40, iter->second->name, WID_TILE_BUTTON, iter->second->textureSource);
				button->connect(&Editor::messageHandler, this);
				mWindowHandler->addWindow(button);
				mButtonContainer->arrangeObject(button);
			}

			mButtonContainer->setPos(mButtonContainer->getPos() + Vector(0, 200));
			mButtonContainer->setNumComponents(0);

			for(TileIter iter = dataMap.begin(); iter != dataMap.end(); iter++)
			{
				if(iter->second->obstacle)
					continue;

				wButton* button = new wButton(10, 10, 40, 40, iter->second->name, WID_TILE_BUTTON, iter->second->textureSource);
				button->connect(&Editor::messageHandler, this);
				mWindowHandler->addWindow(button);
				mButtonContainer->arrangeObject(button);
			}

			mButtonContainer->setPos(mButtonContainer->getPos() - Vector(0, 200));
		}
		else if(msg.getString() == "Props")
		{
			map<string, ObjectData> dataMap = gObjectHandler->getDataMap();
			mActiveObject.name = (*dataMap.begin()).second.name;

			for(auto iter = dataMap.begin(); iter != dataMap.end(); iter++)
			{
				wButton* button = new wButton(10, 10, 40, 40, iter->second.name, WID_TILE_BUTTON, iter->second.textureSource);
				button->connect(&Editor::messageHandler, this);
				mWindowHandler->addWindow(button);
				mButtonContainer->arrangeObject(button);
			}

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
			createInputBox();
			mActiveObject.type = REGION;
		}
	}
	else if(id == WID_TILE_BUTTON || WID_OBJECT_BUTTON)
	{
		mActiveObject.name = msg.getString();
	}

	return true;
}

void Editor::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		// Enter pressed
		case IDC_ENTER_PRESSED:
		{
			char buffer[256];
			GetWindowText(mhInputBox, buffer, 256);
			string text = string(buffer);
			mSelectedRegion->getTrigger()->setTriggerString(text);
			SetWindowText(mhInputBox, mSelectedRegion->getTrigger()->getTriggerAsString().c_str());
			//MessageBox(0, text.c_str(), 0, 0);
			break;
		}
	}
}

void Editor::createInputBox()
{
	mhInputBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | ES_MULTILINE | ES_AUTOVSCROLL,
        825, 160, 180, 500, gGame->getMainWnd(), (HMENU)IDC_INPUT_BOX, gGame->getAppInst(), NULL);

	// Set the default edit control proc
	DefEditProc = (WNDPROC)SetWindowLong(mhInputBox, GWL_WNDPROC, (DWORD)inputProc);
}