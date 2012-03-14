#ifndef EDITOR_H
#define EDITOR_H

#include <Windows.h>
#include <string>
#include "WindowHandler.h"
#include "enums.h"
#include "Object.h"

class Level;
class wButton;
class wDropDown;
class wContainer;
class wLabel;
class Region;

enum CommandResult
{
	CR_SUCCESS,
	CR_ERROR
};

struct ActiveObject
{
	ActiveObject(string name, ObjectType type) {
		this->name = name;
		this->type = type;
	}

	ActiveObject(){};

	string name;
	ObjectType type;
};

class Editor
{
public:
	Editor(std::string levelName);
	Editor();
	~Editor();

	void update(float dt);
	void draw();

	bool messageHandler(wId id, wMessage msg);
	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void			createInputBox();
private:
	Level			*mLevel;
	ActiveObject	mActiveObject;
	Object*			mMovingObject;
	Vector			mMovingOffset;

	// GUI stuff
	WindowHandler*	mWindowHandler;
	wDropDown*		mDropDown;
	wContainer*		mButtonContainer;
	wLabel*			mTileLabel1;
	wLabel*			mTileLabel2;
	HWND			mhInputBox;

	Vector			mClickedPos;
	Region*			mSelectedRegion;
};

#endif