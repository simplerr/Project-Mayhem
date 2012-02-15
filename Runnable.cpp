#include "Runnable.h"
#include "Input.h"
#include "Graphics.h"

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created
	if(gGame != NULL )
		return gGame->msgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

Runnable::Runnable(HINSTANCE hInstance, std::string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP)
{
	mCaption		= caption;
	mDevType		= devType;
	mRequestedVP	= requestedVP;
	mWidth			= width;
	mHeight			= height;
	mhInstance		= hInstance;
	mhMainWindow	= NULL;
	mPaused			= false;
	mDeviceLost		= false;
	mDeltaSum		= 0;

	ZeroMemory(&md3dPP, sizeof(md3dPP));

	// Init and make the window ready to use
	initWindow();
	initDiRectX();
	setRenderStates();
}

Runnable::~Runnable()
{
	
}

//! Inits Win32
bool Runnable::initWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Create the window with a custom size and make it centered
	// NOTE: WS_CLIPCHILDREN Makes the area under child windows not be displayed. (Useful when rendering DirectX and using windows controls).
	RECT R = {0, 0, mWidth, mHeight};
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	mhMainWindow = CreateWindow("D3DWndClassName", mCaption.c_str(), 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, GetSystemMetrics(SM_CXSCREEN)/2-(mWidth/2),
		GetSystemMetrics(SM_CYSCREEN)/2-(mHeight/2), mWidth, mHeight, 
		0, 0, mhInstance, 0); 

	if(!mhMainWindow )
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(mhMainWindow, SW_SHOW);
	UpdateWindow(mhMainWindow);

	return true;
}

//! Inits DirectX
bool Runnable::initDiRectX()
{
	// Create the IDirect3D9 object.
    md3dObject = Direct3DCreate9(D3D_SDK_VERSION);
	if( !md3dObject )
	{
		MessageBox(0, "DiRect3DCreate9 FAILED", 0, 0);
		PostQuitMessage(0);
	}


	// Verify hardware support for specified formats in windowed and full screen modes.
	D3DDISPLAYMODE mode;
	md3dObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	HR(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, mode.Format, mode.Format, true));
	HR(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));

	// Check for requested vertex processing and pure device.
	D3DCAPS9 caps;
	HR(md3dObject->GetDeviceCaps(D3DADAPTER_DEFAULT, mDevType, &caps));

	DWORD devBehaviorFlags = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		devBehaviorFlags |= mRequestedVP;
	else
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// If pure device and HW T&L supported
	if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
		devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
			devBehaviorFlags |= D3DCREATE_PUREDEVICE;

	// Fill out the D3DPRESENT_PARAMETERS structure
	md3dPP.BackBufferWidth            = 0; 
	md3dPP.BackBufferHeight           = 0;
	md3dPP.BackBufferFormat           = D3DFMT_UNKNOWN;
	md3dPP.BackBufferCount            = 1;
	md3dPP.MultiSampleType            = D3DMULTISAMPLE_NONE;
	md3dPP.MultiSampleQuality         = 0;
	md3dPP.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	md3dPP.hDeviceWindow              = mhMainWindow;
	md3dPP.Windowed                   = true;
	md3dPP.EnableAutoDepthStencil     = true; 
	md3dPP.AutoDepthStencilFormat     = D3DFMT_D24S8;
	md3dPP.Flags                      = 0;
	md3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	md3dPP.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;


	// Create the device
	HR(md3dObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// primary adapter
		mDevType,				// device type
		mhMainWindow,			// window associated with device
		devBehaviorFlags,		// vertex processing
	    &md3dPP,				// present parameters
	    &gd3dDevice));			// return created device to the global

	return true;
}

HINSTANCE Runnable::getAppInst()
{
	return mhInstance;
}
	
HWND Runnable::getMainWnd()
{
	return mhMainWindow;
}
D3DPRESENT_PARAMETERS Runnable::getPresParams()
{
	return md3dPP;
}

//! The game loop. Calls the inherited update() and draw() each frame
int Runnable::run()
{
	float time = 0.0f;
	MSG  msg;
    msg.message = WM_NULL;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff
		// If the application is paused then free some CPU cycles to other 
		// applications and then continue on to the next frame
		if(mPaused)
		{
			Sleep(20);
			//continue;
		}

		if(!mDeviceLost)
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;
				
			if(mDeltaSum < (float)1/60)	{
				mDeltaSum += dt;
			}
			else	{
				// Update everything
				if(mDeltaSum < .25) // FPS is capped - todo
					update(time, mDeltaSum);

				// Draw everything
				drawAll();				

				// Get device state
				mDeviceLost = isDeviceLost();

				time += mDeltaSum;
				mDeltaSum = 0;
			}	
			// Prepare for next iteration: The current time stamp becomes the previous time stamp for the next iteration
			prevTimeStamp = currTimeStamp;
		}
		else	{
			Sleep(20);
		}
    }
	return (int)msg.wParam;
}

//! Handels all window messages. Gets connected in Runnable::initWindow()
LRESULT Runnable::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Is the application in a minimized or maximized state?
	static bool minOrMaxed = false;

	RECT clientRect = {0, 0, 0, 0};
	switch( msg )
	{
	// WM_ACTIVE is sent when the window is activated or deactivated
	// We pause the game when the main window is deactivated and 
	// unpause it when it becomes active
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
			mPaused = true;
		else
			mPaused = false;
		return 0;


	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		if( gd3dDevice )
		{
			md3dPP.BackBufferWidth  = LOWORD(lParam);
			md3dPP.BackBufferHeight = HIWORD(lParam);

			if( wParam == SIZE_MINIMIZED )
			{
				mPaused = true;
				minOrMaxed = true;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				mPaused = false;
				minOrMaxed = true;
				onLostDevice();
				//HR(gd3dDevice->Reset(&md3dPP));
				onResetDevice();
			}
			// Restored is any resize that is not a minimize or maximize.
			// For example, restoring the window to its default size
			// after a minimize or maximize, or from dragging the resize
			// bars.
			else if( wParam == SIZE_RESTORED )
			{
				mPaused = false;

				// Are we restoring from a mimimized or maximized state, 
				// and are in windowed mode?  Do not execute this code if 
				// we are restoring to full screen mode.
				if( minOrMaxed && md3dPP.Windowed )
				{
					onLostDevice();
					//HR(gd3dDevice->Reset(&md3dPP));
					onResetDevice();
				}
				minOrMaxed = false;
			}
		}
		return 0;


	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		GetClientRect(mhMainWindow, &clientRect);
		md3dPP.BackBufferWidth  = clientRect.right;
		md3dPP.BackBufferHeight = clientRect.bottom;
		onLostDevice();
		//HR(gd3dDevice->Reset(&md3dPP));
		onResetDevice();

		return 0;

	// WM_CLOSE is sent when the user presses the 'X' button in the
	// caption bar menu.
	case WM_CLOSE:
		DestroyWindow(mhMainWindow);		
		return 0;

	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		return 0;
	case WM_COMMAND:
		return 0;
	}

	return DefWindowProc(mhMainWindow, msg, wParam, lParam);
}

bool Runnable::isDeviceLost()
{
	// Get the state of the graphics device.
	HRESULT hr = gd3dDevice->TestCooperativeLevel();

	// If the device is lost and cannot be reset yet then
	// sleep for a bit and we'll try again on the next 
	// message loop cycle.
	if( hr == D3DERR_DEVICELOST )
	{		
		Sleep(20);
		return true;
	}
	// Driver error, exit.
	else if( hr == D3DERR_DRIVERINTERNALERROR )
	{
		MessageBox(0, "Internal Driver Error...Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	// The device is lost but we can reset and restore it.
	else if( hr == D3DERR_DEVICENOTRESET )
	{
		onLostDevice();
		HR(gd3dDevice->Reset(&md3dPP));
		onResetDevice();
		return false;
	}
	else
		return false;
}

void Runnable::message(std::string msg)
{
	MessageBox(0, msg.c_str(), "Flex Engine", 0);
}

void Runnable::setFps(float fps)
{
	mFPS = fps;
}

int	Runnable::getScreenWidth()
{
	return mWidth;
}

int	Runnable::getScreenHeight()
{
	return mHeight;
}
void showMsg(std::string msg)	{
	MessageBox(gGame->getMainWnd(), msg.c_str(), 0, 0);
}

void Runnable::setRenderStates()
{
	// This code sets texture filters, which helps to smooth out distortions
	// when you scale a texture.  
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));

	gd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// Indicates that we are using 2D texture coordinates.
	HR(gd3dDevice->SetTextureStageState(
		0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2));

	// No lightning in order for the colors to show
	gd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void Runnable::drawAll()
{
	HR(gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

	// Clears the backbuffer and depth buffer
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));	// 0xffffffff

	HR(gd3dDevice->BeginScene());

	// Rendering of all sprites
	// Even for none sprites this means that everthing gets drawn in screen coordinates
	gGraphics->getSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);	// :NOTE: Can be reason for noraml SetState(....POINT) didnt work!!

	// The function that contains all the drawing
	draw();

	gGraphics->getSpriteHandler()->Flush();
	gGraphics->getSpriteHandler()->End();

	// Rendering af all none-sprites

	HR(gd3dDevice->EndScene());

	// Present the backbuffer
	HR(gd3dDevice->Present(0, 0, 0, 0));
}