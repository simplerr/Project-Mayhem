//=============================================================================
// DiRectInput.h by Frank Luna (C) 2005 All Rights Reserved.
//
// Wraps initialization of immediate mode DiRect Input, and provides 
// information for querying the state of the keyboard and mouse.
//=============================================================================

#ifndef DIRect_INPUT_H
#define DIRect_INPUT_H

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "d3dUtil.h"

class Vector;

namespace common
{

/// Wraps up input
class DirectInput
{
public:
	DirectInput(HWND hwnd, DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
	~DirectInput();

	void poll();
	bool keyDown(char key);
	bool keyPressed(char key);
	bool keyReleased(char key);
	bool mouseButtonDown(int button);
	bool mouseButtonPressed(int button);
	bool mouseButtonReleased(int button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

	char ScanToChar(DWORD scanCode) const;
	char getInput(void);

	void	updateCursor(void);
	void	drawCursor(void);

	float	getCursorX(void);
	float	getCursorY(void);
	Vector	getCursorPos(void);
	void	setCursorX(float x);
	void	setCursorY(float y);

	void	restoreCursor(void);
	void	drawCursorPos(void);
	Vector	getCursorInworld(void);

	bool	cursorInsideRect(RECT r);
private:
	// Make private to prevent copying of members of this class.
	DirectInput(const DirectInput& rhs);
	DirectInput& operator=(const DirectInput& rhs);
		
private:
	IDirectInput8*       mDInput;

	IDirectInputDevice8* mKeyboard;
	char                 mKeyboardState[256]; 
	char				 mLastKeyboardState[256];

	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;
	DIMOUSESTATE2        mLastMouseState;

	float				mCursorX;
	float				mCursorY;
	HWND				mHwnd;

	IDirect3DTexture9*	mCursorTexture;
};	// Class
};	// Namespace

// Global
extern common::DirectInput* gDInput;
#endif