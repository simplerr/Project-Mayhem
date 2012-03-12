#include <boost\lexical_cast.hpp>
#include "WindowHandler.h"
#include "Input.h"
#include "Vector.h"
#include "Rect.h"
#include "Graphics.h"
#include "wComponent.h"

WindowHandler::WindowHandler()
{
	mActiveWindow = NULL;
}
	
WindowHandler::~WindowHandler()
{

}

void WindowHandler::addWindow(wComponent* window)
{
	mWindowList.push_back(window);
}
	
void WindowHandler::removeWindow(int id)
{
	// Loop through all objects and find out which one to delete
	int i = 0;
	std::vector<wComponent*>::iterator itr =  mWindowList.begin();
	while(itr != mWindowList.end() && i < mWindowList.size())
	{
		if(mWindowList[i]->getId() == id)
		{
			delete mWindowList[i];		// viktigt att deleta innan!
			mWindowList[i] = NULL;
			itr = mWindowList.erase(itr);	
			break;
		}
		else	{
			itr++;
			i++;
		}
	}

	int adas = 1;
}
	
void WindowHandler::removeWindow(wComponent* window)
{
	// :TODO:
}

void WindowHandler::update(float dt)
{
	// :NOTE: Cheesy
	if(mActiveWindow != NULL)	{
		if(gInput->keyPressed(VK_LBUTTON) && gMath->pointInsideRect(gInput->mousePosition(), mActiveWindow->getRect()))	{
			mActiveWindow->onPress();
		}
	}

	// Get user input and send to the right window component
	bool insideAny = false;
	for(int j = 5; j >= 0; j--)
	{
		if(insideAny == true)
			break;

		for(int i = 0; i < mWindowList.size(); i++)
		{
			if(mWindowList[i]->getZ() == j)
			{
				// If the didn't press inside any window the active window is set to NULL
				// :NOTE: In some cases you might want to keep the active window
				if(mouseInside(mWindowList[i]->getRect()) && gInput->keyPressed(VK_LBUTTON))	{
					insideAny = true;

					if(mActiveWindow != mWindowList[i] && mActiveWindow != NULL)
						mActiveWindow->setActive(false);

					if(mActiveWindow != mWindowList[i])	{
						mActiveWindow = mWindowList[i];
						if(mActiveWindow->onPress())
							return;
					}
					break;
				}
				else if(mouseInside(mWindowList[i]->getRect()))
					mWindowList[i]->onHoover();
			}
		}
	}

	if(!insideAny && mActiveWindow != NULL && gInput->keyPressed(VK_LBUTTON))
		mActiveWindow->setActive(false);

	// Update each component
	/*for(int i = 0; i < mWindowList.size(); i++)
	{
		mWindowList[i]->update(dt);
	}*/

	if(mActiveWindow != NULL)
		mActiveWindow->update(dt);
}
	
void WindowHandler::draw()
{
	for(int j = 0; j <= 5; j++)
	{
		// Draw each component
		for(int i = 0; i < mWindowList.size(); i++)
		{
			if(mWindowList[i]->getZ() == j)
				mWindowList[i]->draw();
		}
	}
}

// Returns true if the mouse is inside the rect
bool WindowHandler::mouseInside(Rect rect)
{
	// Mouse position
	Vector p = gInput->mousePosition();

	if(p.x < rect.right && p.x > rect.left && p.y > rect.top && p.y < rect.bottom)
		return true;
	else
		return false;
}