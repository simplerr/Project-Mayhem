#pragma once

#include "Projectile.h"

class Object;
class Rect;

class Scrap 
{
public:
	Scrap();
	~Scrap();
	
	Object* tileObject; // Temporary test object, with position and dimensions of the tile
	Rect*	rect;
	Rect*	editorRect;
	bool	editingRegion;
	ProjectileData basicProjectile;
};

extern Scrap* gScrap;