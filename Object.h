#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "d3dUtil.h"
#include "Vector.h"
#include "cPolygon.h"

using namespace std;

class Level;
class MTV;

enum ObjectType
{
	STRUCTURE,	// Any structure/wall
	PROP,		// Objects you can't pick up, just for visibility
	LOOT,		// Objects you can pick up
	ENEMY,		// Enemy o.0
	SFX,		// Special effect, no collision detection
	PROJECTILE,	// Projectile
	PLAYER,		// Player
	TILE,
	GOLD_COIN,
	REGION,
	HP_POTION,
	ENERGY_POTION
};

enum Layer {
	BOTTOM,
	MIDDLE,
	TOP
};

class ObjectData
{
public:
	float health;
	float width, height;
	string textureSource;
	string name;
	cPolygon polygon;
	Layer drawLayer;
	bool collides;
	// :TODO: Fill with more...
};

class Object
{
public:
	Object(float x, float y, int width, int height, ObjectType type = STRUCTURE, std::string textureSource = "Data\\imgs\\standard_box.bmp");
	virtual ~Object();

	virtual void update(float dt);
	virtual void draw();
	virtual void move(float dx, float dy);
	virtual bool handleCollision(Object* collider, MTV* mtv) {return false;};

	void setPolygon(cPolygon polygon)	{mPolygon = polygon;}

	void rotate(float rotation);
	void resetRotation()				{mPolygon.resetRotation();}

	void setID(int id);
	void setRotation(float rad);
	void setPos(Vector pos);
	void setCollidable(bool collides)	{mCollidable = collides;}
	void setLayer(Layer layer)			{mLayer = layer;}
	void setLevel(Level* level)			{mActiveLevel = level;}
	void setAlive(bool alive)			{mAlive = alive;}
	void setType(ObjectType type)		{mType = type;}
	void setOwnerId	(int id)			{mOwnerId = id;}
	void setDetailedCollision(bool detailed) {mDetailedCollision = detailed;}
	void setTexture(string source);

	IDirect3DTexture9*	getTexture();
	string				getTextureSource();
	Vector		getPos();
	int			getID();
	bool		getColides()		{return mCollidable;}
	cPolygon&	getPolygon()		{return mPolygon;}
	Layer		getLayer()			{return mLayer;}
	Level*		getLevel()			{return mActiveLevel;}
	float		getRotation()		{return mPolygon.getRotation();}
	bool		getAlive()			{return mAlive;}
	ObjectType	getType()			{return mType;}
	int			getOwnerId()		{return mOwnerId;}
	bool		detailedCollision() {return mDetailedCollision;}
	
	Rect		getRect();
	Rect		getBoundingBox();

	// :TODO: gets and sets
	// :TODO: an object should be made of points, make a Shape class
private:
	// Will probably have a shape made of points once we add collisions
	cPolygon			mPolygon;
	float				mRotation;
	int					mWidth;
	int					mHeight;
	int					mID;
	IDirect3DTexture9*	mTexture;
	bool				mCollidable;
	Layer				mLayer;
	Level*				mActiveLevel;
	bool				mAlive;
	ObjectType			mType;
	int					mOwnerId;
	string				mTextureSource;
	bool				mDetailedCollision;
};	// Class

#endif