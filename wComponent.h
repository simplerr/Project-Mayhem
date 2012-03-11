#ifndef WCOMPONENT_H
#define WCOMPONENT_H

#include "Vector.h"
#include "wMessage.h"
#include "enums.h"
#include "d3dUtil.h"
//#include <boost\function.hpp>
//#include <boost\bind.hpp>

class Rect;

class wComponent
{
public:
	wComponent(int x, int y, int width, int height, wId id = WID_STANDARD);
	~wComponent();

	virtual void update(float dt);
	virtual void draw();

	/* Event functions */
	virtual bool onPress();
	virtual void onHoover();
	virtual void onText();
	virtual void setActive(bool active);

	Rect getRect();
	
	bool getActive();

	void setPos(Vector pos)		{mPosition = pos;}
	void setWidth(int width)	{mWidth = width;}
	void setHeight(int height)	{mHeight = height;}
	void setId(wId id)			{mId = id;}
	void setZ(int z)			{mZ = z;}
	void setTexture(string textureSource);

	Vector	getPos()			{return mPosition;}
	int		getWidth()			{return mWidth;}
	int		getHeight()			{return mHeight;}
	wId		getId()				{return mId;}
	int		getZ()				{return mZ;}
private:
	IDirect3DTexture9* mTexture;
	Vector	mPosition;
	wId		mId;
	bool	mActive;
	int		mWidth;
	int		mHeight;
	int		mZ;
};

#endif