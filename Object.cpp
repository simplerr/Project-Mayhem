#include "Object.h"
#include "Graphics.h"
#include "Vector.h"
#include "cPolygon.h"

Object::Object(float x, float y, int width, int height, ObjectType type, std::string textureSource)
{
	// :NOTE: The order is important!
	mPolygon = cPolygon(Vector(x, y, 0));
	mPolygon.addPoint(-width/2, -height/2);
	mPolygon.addPoint(-width/2, height/2);
	mPolygon.addPoint(width/2, height/2);
	mPolygon.addPoint(width/2, -height/2);

	mWidth = width;
	mHeight = height;
	mTexture = gGraphics->loadTexture(textureSource);
	mTextureSource = textureSource;
	mRotation = 0;
	setAlive(true);
	setType(type);
	setLayer(MIDDLE);
}

Object::~Object()
{
	// Release the texture, COM object don't get deleted you instead release them
	ReleaseCOM(mTexture);
}

void Object::setID(int id)
{
	mID = id;
}

int Object::getID()
{
	return mID;
}

void Object::update(float dt)
{
	// Inherited objects will add functionality here
}
	
void Object::move(float dx, float dy)
{
	mPolygon.setPos(Vector(mPolygon.getPos().x + dx, mPolygon.getPos().y + dy));
}

void Object::draw()
{
	//gGraphics->drawTexture(mTexture, mPolygon->getPos().x, mPolygon->getPos().y, mWidth, mHeight, mRotation);
	gGraphics->drawTexturedPolygon(mPolygon, getTexture());
	//mPolygon.drawDebug();
}

void Object::rotate(float rotation)
{
	mPolygon.rotate(rotation);
}

void Object::setRotation(float rad)
{
	mRotation = rad;
	mPolygon.rotate(rad);
}

void Object::setPos(Vector pos)
{
	mPolygon.setPos(pos);
}

Vector Object::getPos()
{
	return mPolygon.getPos();
}

IDirect3DTexture9* Object::getTexture()
{
	return mTexture;	
}

Rect Object::getRect()
{
	Rect rect;
	rect.left = getPos().x - mWidth/2;
	rect.right = getPos().x + mWidth/2;
	rect.top = getPos().y - mHeight/2;
	rect.bottom = getPos().y + mHeight/2;

	return rect;
}

Rect Object::getBoundingBox()
{
	return mPolygon.getBoundingBox();
}

string Object::getTextureSource()
{
	return mTextureSource;
}