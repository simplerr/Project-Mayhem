#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include "WindowHandler.h"
#include "enums.h"
#include "Object.h"

class Level;
class wButton;
class wDropDown;
class wContainer;

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

private:
	Level			*mLevel;
	ActiveObject	mActiveObject;

	// GUI stuff
	WindowHandler*	mWindowHandler;
	wDropDown*		mDropDown;
	wContainer*		mButtonContainer;

	Vector			mClickedPos;
};

#endif