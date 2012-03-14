#pragma once

#include "Projectile.h"
#include "Graphics.h"

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
	IDirect3DTexture9* shadow;
};

extern Scrap* gScrap;