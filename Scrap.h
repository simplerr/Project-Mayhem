#pragma once

#include "Projectile.h"

class Object;
class Rect;


class Scrap {
public:
	// Temporary test object, with position and dimensions of the tile
	Object* tileObject;
	Rect*	rect;
	Rect*	editorRect;
	bool	editingRegion;
	ProjectileData basicProjectile;

	Scrap();
	~Scrap();

};

extern Scrap* gScrap;