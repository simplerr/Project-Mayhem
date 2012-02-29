//=============================================================================
// DiRectInput.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "d3dUtil.h"
#include "DirectInput.h"
#include "Graphics.h"
#include "Runnable.h"
#include "Vector.h"
#include "CameraManager.h"

using namespace common;

DirectInput::DirectInput(HWND hwnd, DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	mHwnd = hwnd;

	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	HR(DirectInput8Create(gGame->getAppInst(), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&mDInput, 0));

	HR(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	HR(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyboard->SetCooperativeLevel(gGame->getMainWnd(), keyboardCoopFlags));
	HR(mKeyboard->Acquire());

	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(gGame->getMainWnd(), mouseCoopFlags));
	HR(mMouse->Acquire());

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(mHwnd, &mousePos);

	mCursorX = mousePos.x;
	mCursorY = mousePos.y;

	mCursorTexture = gGraphics->loadTexture("Data\\imgs\\cursor.bmp");
}

DirectInput::~DirectInput()
{
	ReleaseCOM(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	ReleaseCOM(mKeyboard);
	ReleaseCOM(mMouse);
	ReleaseCOM(mCursorTexture);
}

void DirectInput::poll()
{
	// Poll keyboard.
	memcpy(mLastKeyboardState, mKeyboardState, sizeof(mKeyboardState));
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState); 
	if( FAILED(hr) )
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		 // Try to acquire for next time we poll.
		hr = mKeyboard->Acquire();
	}

	// Poll mouse.
	memcpy(&mLastMouseState, &mMouseState, sizeof(mMouseState));
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState); 
	if( FAILED(hr) )
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to acquire for next time we poll.
		hr = mMouse->Acquire(); 
	}
}

bool DirectInput::keyDown(char key)
{
	return (mKeyboardState[key] & 0x80) != 0;
}
// fixa till ... >_>
bool DirectInput::keyPressed(char key)
{	
	return (!mLastKeyboardState[key] && mKeyboardState[key] & 0x80 ) != 0;	
}
// true if button is down
bool DirectInput::mouseButtonDown(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}
// true if button is pressed
bool DirectInput::mouseButtonPressed(int button)
{
	return (!mLastMouseState.rgbButtons[button] && mMouseState.rgbButtons[button] & 0x80) != 0;
}
// true if button is released
bool DirectInput::mouseButtonReleased(int button)
{
	if((mLastMouseState.rgbButtons[button] & 0x80) != 0 && (mMouseState.rgbButtons[button] & 0x80) == 0)
		return true;
	else
		return false;
}

bool DirectInput::keyReleased(char key)
{
	if((mLastKeyboardState[key] & 0x80) != 0 && (mKeyboardState[key] & 0x80) == 0)
		return true;
	else
		return false;
}

float DirectInput::mouseDX()
{
	return (float)mMouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)mMouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}

char DirectInput::ScanToChar(DWORD scanCode) const
{
	//obtain keyboard information
	static HKL layout = GetKeyboardLayout(0);
	static UCHAR keyboardState[256];
	if (GetKeyboardState(keyboardState) == false)
		return 0;

	//translate keyboard press scan code identifier to a char
	UINT vk = MapVirtualKeyEx(scanCode, 1, layout);
	USHORT asciiValue;
	ToAscii(vk, scanCode, keyboardState, &asciiValue, 0);

	return 'a';
}


char DirectInput::getInput(void)
{
	for(int i = 0;i<255;i++){
		if(mKeyboardState[i] != mLastKeyboardState[i])	{
			if(mKeyboardState[i] & 0x80)	{
				char tmp;
				tmp = ScanToChar(DIK_A);
				return tmp;//mKeyboardState[i];
			}
		}
	}		
	return 'n';
}

void DirectInput::updateCursor(void)
{
	RECT windowRect;
	GetWindowRect(mHwnd, &windowRect);

	/* stop updating when outside the screen */
	if(mCursorX <= -windowRect.left)	{
		if(mMouseState.lX > 0)
			mCursorX += mMouseState.lX;
		else
			mCursorX = -windowRect.left - 8;
	}
	/* this only works on none-dual screens */
	else if(mCursorX >= GetSystemMetrics(SM_CXSCREEN))	{
		if(mMouseState.lX < 0)
			mCursorX += mMouseState.lX;
		else
			mCursorX = GetSystemMetrics(SM_CXSCREEN);
	}
	else
		mCursorX += mMouseState.lX;

	if(mCursorY <= -windowRect.top)	{
		if(mMouseState.lY > 0)
			mCursorY += mMouseState.lY;
		else
			mCursorY = - windowRect.top - 30;
	}
	else if(mCursorY >= GetSystemMetrics(SM_CYSCREEN) - windowRect.top - 30)	{
		if(mMouseState.lY < 0)
			mCursorY += mMouseState.lY;
		else
			mCursorY = GetSystemMetrics(SM_CYSCREEN) - windowRect.top - 30;
	}
	else
		mCursorY += mMouseState.lY;

	/* need to set the cursor position */
	/* it's invisible but in order to press the close button this is needed*/
	SetCursorPos(mCursorX + windowRect.left + 8, mCursorY + windowRect.top + 30);
}

float DirectInput::getCursorX(void)
{
	return mCursorX;
}
	
float DirectInput::getCursorY(void)
{
	return mCursorY - 16;
}

void DirectInput::drawCursorPos(void)
{
	//char buffer[256];
	//sprintf(buffer, "Mouse x: %.2f\nMouse y: %.2f", getCursorX(), getCursorY(), mouseDX(), mouseDY());
	//gGraphics->drawText(buffer, 10, 400); 
}

void DirectInput::drawCursor(void)
{
	gGraphics->drawTexture(mCursorTexture, mCursorX+gCameraManager->gameCamera()->getCameraCorner().x, mCursorY+gCameraManager->gameCamera()->getCameraCorner().y, 32, 32);
	//gGraphics->drawTexture(mCursorTexture, mCursorX, mCursorY, 32, 32);
}

void DirectInput::setCursorX(float x)
{
	mCursorX = x;
}

void DirectInput::setCursorY(float y)
{
	mCursorY = y;
}

void DirectInput::restoreCursor(void)
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(mHwnd, &mousePos);

	mCursorX = mousePos.x;
	mCursorY = mousePos.y;
}

Vector DirectInput::getCursorPos(void)
{
	Vector tmpPoint;
	tmpPoint.x = mCursorX - 12;
	tmpPoint.y = mCursorY - 16;

	return tmpPoint;
}
bool DirectInput::cursorInsideRect(RECT r)
{
	if(mCursorX > r.left && mCursorX < r.right && mCursorY > r.top && mCursorY < r.bottom)
		return true;
	else
		return false;
}

Vector DirectInput::getCursorInworld() 
{
	Vector v;
	v.x = gCameraManager->gameCamera()->getCameraCorner().x+mCursorX - 12;
	v.y = gCameraManager->gameCamera()->getCameraCorner().y+mCursorY - 16;
	return v;
}