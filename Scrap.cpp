#include "Scrap.h"
#include "Object.h"
#include "Rect.h"
#include "Projectile.h"

Scrap::Scrap() 
{
	tileObject = new Object(0, 0, 40, 40, STRUCTURE, "Data\\imgs\\standard_box.bmp");
	tileObject->setID(-1337);
	rect = new Rect(10,10,40,40,0);
	editorRect = new Rect(10,10,10,10,0);
	editingRegion = false;
	basicProjectile = ProjectileData();
	enemyCheck = new Object(0, 0, 40, 40, STRUCTURE, "Data\\imgs\\standard_box.bmp");
	shadow = gGraphics->loadTexture("Data\\imgs\\shadow.png");
}

Scrap::~Scrap() 
{
	delete tileObject;
	delete rect;
	delete editorRect;
}