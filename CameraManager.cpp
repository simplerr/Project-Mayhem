#include "CameraManager.h"

CameraManager *gCameraManager	= 0;

CameraManager::CameraManager()
{
	mGameCamera = new Camera(400, 300, 800, 600);
	mGuiCamera = new Camera(1300, 450, 200, 900);
}

CameraManager::~CameraManager()
{
	delete mGameCamera;
	delete mGuiCamera;
}

void CameraManager::setCamera(Cam camera)
{
	if(camera == GAME_CAMERA)	{
		if(!mGameCamera->getActive())	{
			mGameCamera->activate(true);
			mGuiCamera->activate(false);
		}
	}
	else if(camera == GUI_CAMERA)	{
		if(!mGuiCamera->getActive())	{
			mGuiCamera->activate(true);
			mGameCamera->activate(false);
		}
	}
}

Camera* CameraManager::gameCamera(void)
{
	return mGameCamera;
}

Camera* CameraManager::guiCamera(void)
{
	return mGuiCamera;
}