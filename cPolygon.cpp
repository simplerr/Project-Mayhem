#include "cPolygon.h"
#include "Vector.h"
#include "d3dUtil.h"
#include "Graphics.h"

cPolygon::cPolygon(Vector origin)
{
	mOrigin = origin;
	mRotation = 0;
	mBoundingBox = Rect(9999999, 0, 9999999, 0);
	mDrawRect = Rect(9999999, 0, 9999999, 0);
}
	
cPolygon::cPolygon(Vector origin, Vector point)
{
	mOrigin = origin;
	addPoint(point);
}
	
// :TODO: Copy ctr
cPolygon::~cPolygon()
{

}

void cPolygon::addPoint(Vector point)
{
	// :TODO: Add them in the right order
	mVertices.push_back(point);

	mBoundingBox.left = min(point.x, mBoundingBox.left);
	mBoundingBox.right = max(point.x, mBoundingBox.right);
	mBoundingBox.top = min(point.y, mBoundingBox.top);
	mBoundingBox.bottom = max(point.y, mBoundingBox.bottom);

	mDrawRect.left = min(point.x, mDrawRect.left);
	mDrawRect.right = max(point.x, mDrawRect.right);
	mDrawRect.top = min(point.y, mDrawRect.top);
	mDrawRect.bottom = max(point.y, mDrawRect.bottom);
}
	
void cPolygon::drawDebug()
{
	Rect tmpRect = mBoundingBox;
	tmpRect.left += getPos().x;
	tmpRect.right += getPos().x;
	tmpRect.top += getPos().y;
	tmpRect.bottom += getPos().y;

	gGraphics->drawRect(tmpRect, D3DCOLOR_ARGB(60, 0, 0, 200));

	drawPoints();
}

void cPolygon::drawPoints()
{
	for(int i = 0; i < mVertices.size(); i++)	{
		gGraphics->drawRect(mVertices[i].x + getPos().x, mVertices[i].y + getPos().y, 10, 10, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
}

void cPolygon::addPoint(Vector point, int listPos)
{
	// :TODO: Add them in the right order
	mVertices.push_back(point);
}

void cPolygon::addPoint(float x, float y)
{
	addPoint(Vector(x, y));
}

void cPolygon::addPoint(float x, float y, int listPos)
{
	addPoint(Vector(x, y), listPos);
}

Vector cPolygon::getPos()
{
	return mOrigin;
}

void cPolygon::setPos(Vector pos)
{
	mOrigin = pos;
}

void cPolygon::rotate(float rotation)
{
	// Rotate the shape with D3DXVec4Transform
	D3DXMATRIX rotationMatrix;
	D3DXVECTOR4 vector;
	vector.z = 0;
	vector.w = 1;
	D3DXMatrixRotationZ(&rotationMatrix, rotation);

	// So the bounding box will get updated properly
	// resetBoundingBox();

	mBoundingBox = Rect(9999999, 0, 999999, 0);
	for(int i = 0; i < mVertices.size(); i++)	
	{
		vector.x = mVertices[i].x;	// Specifies axis to rotate around (local coordinates)
		vector.y = mVertices[i].y;	// Specifies axis to rotate around (local coordinates)

		D3DXVec4Transform(&vector,  &vector, &rotationMatrix);

		mVertices[i].x = vector.x;
		mVertices[i].y = vector.y;

		mBoundingBox.left = min(mVertices[i].x, mBoundingBox.left);
		mBoundingBox.right = max(mVertices[i].x, mBoundingBox.right);
		mBoundingBox.top = min(mVertices[i].y, mBoundingBox.top);
		mBoundingBox.bottom = max(mVertices[i].y, mBoundingBox.bottom);
	}

	mRotation += rotation;
	if(mRotation > 2*PI)
		mRotation = rotation;
}

float cPolygon::getRotation()
{
	return mRotation;
}

void cPolygon::resetRotation()
{
	rotate(-mRotation);
}

Rect cPolygon::getBoundingBox()
{
	Rect rect = mBoundingBox;

	rect.left += mOrigin.x;
	rect.right += mOrigin.x;
	rect.top += mOrigin.y;
	rect.bottom += mOrigin.y;

	return rect;
}