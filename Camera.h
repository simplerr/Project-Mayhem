#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"

class Camera
{
public:
	Camera(int x, int y, int width, int height);
	~Camera();

	D3DVIEWPORT9 getViewport(void)	{return mViewport;};
	D3DXMATRIX getView(void)		{return mView;};
	D3DXMATRIX getProj(void)		{return mProj;};
	int getX(void)					{return mPos.x;};
	int getY(void)					{return mPos.y;};
	int getWidth(void)				{return mWidth;};
	int getHeight(void)				{return mHeight;};
	int getOffset(void)				{return mPos.x - 600;};

	void setX(int x)				{mPos.x = x;};
	void setY(int y)				{mPos.y = y;};

	void activate(bool b);
	bool getActive(void)	{ return mActive;};

	void buildViewMatrix(void);
	void buildProjectionMatrix(void);
	void buildViewport(void);

	void restore(void);
	void addMovement(float dx, float dy);
	void move();

	D3DXVECTOR2 getCameraCorner(void);
private:
	D3DVIEWPORT9 mViewport;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	D3DXVECTOR2 mPos;
	D3DXVECTOR2 mViewportPos;
	int mWidth;
	int mHeight;

	bool mActive;

	float mDX;
	float mDY;
};

#endif