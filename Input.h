#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include <dinput.h>

#include "Vector.h"
#include "d3dUtil.h"

//! Wrapper for input with keyboard and mouse.
class Input
{
public:
	Input();
	~Input();

	void update(float dt);
	void draw();
	void poll();

	bool keyPressed(int key);
	bool keyDown(int key);
	bool keyReleased(int key);

	float	mouseDx();
	float	mouseDy();
	float	mouseDz();
	Vector	mousePosition();
	void	setMousePosition(Vector pos);

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	unsigned char mLastKeyState[256];
	unsigned char mKeyState[256];

	IDirectInput8*       mDInput;
	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;
	DIMOUSESTATE2        mLastMouseState;

	Vector mMousePosition;
	float mDx, mDy;
};

// Global
extern Input* gInput;

#endif