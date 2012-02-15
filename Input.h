#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>

#include "Vector.h"

//! Wrapper for input with keyboard and mouse.
class Input
{
public:
	Input();
	~Input();

	void update(float dt);
	void draw();

	bool keyPressed(int key);
	bool keyDown(int key);
	bool keyReleased(int key);

	float	mouseDx();
	float	mouseDy();
	Vector	mousePosition();
	void	setMousePosition(Vector pos);

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	unsigned char mLastKeyState[256];
	unsigned char mKeyState[256];

	Vector mMousePosition;
	float mDx, mDy;
};

// Global
extern Input* gInput;

#endif