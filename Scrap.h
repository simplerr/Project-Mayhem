#pragma once

class Object;
class Rect;


class Scrap {
public:
	// Temporary test object, with position and dimensions of the tile
	Object* tileObject;
	Rect*	rect;

	Scrap();
	~Scrap();

};

extern Scrap* gScrap;