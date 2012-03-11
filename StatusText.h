#pragma once

#include <string>
#include "Vector.h"
#include "enums.h"
#include "d3dUtil.h"
using namespace std;

//! Draws messages explaining what the player did wrong.
class StatusText
{
public:
	StatusText(string source, float x, float y, int width, int height, float time = 2.0f);
	~StatusText();

	void draw();
	void update(float dt);
	void show(float time);
private:
	Vector	mPos;
	IDirect3DTexture9* mTexture;
	float	mTime;
	int		mWidth, mHeight;
};