#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <Windows.h>
#include <string>
#include "d3dUtil.h"

void showMsg(std::string msg);

//! Handles window init, DirectX init and has a game loop that runs at given FPS.
class Runnable
{
public:
	//! Creates a window ready for DirectX drawing.
	Runnable(HINSTANCE hInstance, std::string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP);
	virtual ~Runnable();
	
	//! Sets the default render states.
	void setRenderStates();

	//! Everything neccessary has been done in the constructor. Add custom inits here.
	virtual void init() = 0;

	//! Updates all game content each frame.
	virtual void update(float t, float dt) = 0;

	//! Sets up the neccessary things and calls draw().
	void drawAll();

	//! Draws all game content each frame.
	virtual void draw() = 0;

	//! Sets the FPS.
	void setFps(float fps);

	//! The game loop. Calls the inherited update() and draw() each frame.
	int run();

	//! Handels all window messages. Gets connected in Engine::initWindow().
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	//! Wraps up MessageBox(...)
	void message(std::string msg);

	HINSTANCE				getAppInst();
	HWND					getMainWnd();
	D3DPRESENT_PARAMETERS	getPresParams();
	int						getScreenWidth();
	int						getScreenHeight();

	bool			isDeviceLost();
	virtual void	onLostDevice() {}
	virtual void	onResetDevice() {}

private:
	bool initWindow();
	bool initDiRectX();

private:
	// Application, Windows and DiRectX data members
	std::string		mCaption;
	D3DDEVTYPE		mDevType;
	DWORD			mRequestedVP;
	int				mWidth, mHeight;
	HINSTANCE		mhInstance;
	HWND			mhMainWindow;
	IDirect3D9*		md3dObject;
	D3DPRESENT_PARAMETERS	md3dPP;

	// Members for the game loop
	float	mFPS;
	float	mDeltaSum;
	bool	mDeviceLost;
	bool	mPaused;
};	// Class

// Global
extern Runnable* gGame;

#endif