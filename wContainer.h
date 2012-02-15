#ifndef WCONTAINER_H
#define WCONTAINER_H

#include <vector>
#include "wComponent.h"
using namespace std;

class WindowHandler;

class wContainer : public wComponent
{
public:
	wContainer(float x, float y, int width, int height, wId id);
	~wContainer();
	
	void setItemDimensions(int width, int height);
	void setPadding(int horizontal, int vertical);

	void draw();
	void clearChildren(WindowHandler* windowHandler);
	
	void arrangeObject(wComponent* component);
private:
	vector<wComponent*> mChildren;
	int mItemWidth;
	int mItemHeight;
	int mComponents;
	int mHorizontalPadding;
	int mVerticalPadding;
};

#endif