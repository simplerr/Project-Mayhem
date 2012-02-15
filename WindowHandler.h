#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include <vector>
#include "wComponent.h"

using namespace std;

class Rect;

/* Todo
- Maybe make the coordinates local for the windowhandler? ie x = 50 could be x = 350 in the global window
*/

class WindowHandler
{
public:
	WindowHandler();
	~WindowHandler();

	void addWindow(wComponent* window);
	void removeWindow(int id);
	void removeWindow(wComponent* window);

	void update(float dt);
	void draw();

	bool mouseInside(Rect rect);
private:
	std::vector<wComponent*>	mWindowList;
	wComponent*					mActiveWindow;
	// mActiveWindow;
};

#endif