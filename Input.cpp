#include "Input.h"
#include "Graphics.h"
#include "Game.h"

//! Constructor.
Input::Input()
{
	// Set every key/button 0.
	ZeroMemory(mLastKeyState, sizeof(mLastKeyState));
	ZeroMemory(mKeyState, sizeof(mKeyState));

	HR(DirectInput8Create(gGame->getAppInst(), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&mDInput, 0));

	// Get the cursor starting position.
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	mMousePosition.x = mousePosition.x;
	mMousePosition.y = mousePosition.y;

	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(gGame->getMainWnd(), DISCL_NONEXCLUSIVE|DISCL_FOREGROUND));
	HR(mMouse->Acquire());

	// No delta movement to start with
	mDx = mDy = 0.0f;
}
	
//! Destructor.
Input::~Input()
{
	mMouse->Unacquire();
	ReleaseCOM(mMouse);
	ReleaseCOM(mDInput);
}

//! Update the key state.
/**
@param dt The delta time since the last frame.
*/
void Input::update(float dt)
{
	// Set the old states.
	memcpy(mLastKeyState, mKeyState, sizeof(mKeyState));

	// Get the current keyboard state.
	GetKeyboardState(mKeyState);
}
	
//! Draws debug information.
void Input::draw()
{
	char buffer[256];
	sprintf(buffer, "x: %f, y: %f", mMousePosition.x, mMousePosition.y);
	gGraphics->drawText(string(buffer), 500, 100, SMALL_DX, 20, 0xff00ff00);

	sprintf(buffer, "dx: %f, dy: %f", mDx, mDy);
	gGraphics->drawText(string(buffer), 500, 300, SMALL_DX, 20, 0xff00ff00);
}

//! Updates the mouse position.
/**
@param msg The message.
@param wParam Message data.
@param lParam Message data.
*/
LRESULT Input::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_MOUSEMOVE:	{
		setMousePosition(Vector(LOWORD(lParam), HIWORD(lParam)));
		return 0;
		}
	case WM_LBUTTONDOWN:	{
		//showMsg("doo");
		return 0;
		}
	}
}

//! Checks if the key was pressed.
/**
@param key The to check.
@return True if pressed.
*/
bool Input::keyPressed(int key)
{
	if(key > 255 || key < 0)
		return false;

	// Usees bitwise AND to remove the least significant bit which is set if the key was pressed after the previous call to GetAsyncKeyState.
	// See http://msdn.microsoft.com/en-us/library/windows/desktop/ms646293(v=VS.85).aspx for more info.
	// The same thing in keyDown() and keyReleased()
	return (mKeyState[key] & 0x80) && !(mLastKeyState[key] & 0x80);
}
	
//! Checks if the key is down.
/**
@param key The to check.
@return True if down.
*/
bool Input::keyDown(int key)
{
	if(key > 255 || key < 0)
		return false;

	return mKeyState[key] & 0x80;
}
	
//! Checks if the key was released.
/**
@param key The to check.
@return True if released.
*/
bool Input::keyReleased(int key)
{
	if(key > 255 || key < 0)
		return false;

	return !(mKeyState[key] & 0x80) && (mLastKeyState[key] & 0x80);
}

//! Get the mouse position
/**
@return The mouse position.
*/
Vector Input::mousePosition()
{
	return mMousePosition;
}

//! Set the mouse position.
/**
@param pos The new position.
@note Doesn't acctually change the mouse position on the screen, just the data.
*/
void Input::setMousePosition(Vector pos)
{
	mDx = pos.x - mousePosition().x;
	mDy = pos.y - mousePosition().y;

	mMousePosition = pos;
}

//! Returns horizontal delta movement.
float Input::mouseDx()
{
	return (float)mMouseState.lX;
}

//! Returns vertical delta movement.
float Input::mouseDy()
{
	return (float)mMouseState.lY;
}

float Input::mouseDz()
{
	return (float)mMouseState.lZ;
}

void Input::poll()
{
	// Poll mouse.
	memcpy(&mLastMouseState, &mMouseState, sizeof(mMouseState));
	HRESULT hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState); 
	if( FAILED(hr) )
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to acquire for next time we poll.
		hr = mMouse->Acquire(); 
	}
}