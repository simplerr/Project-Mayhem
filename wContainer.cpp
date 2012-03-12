#include "wContainer.h"
#include "WindowHandler.h"

wContainer::wContainer(float x, float y, int width, int height, wId id) :
	wComponent(x, y, width, height, id)
{
	setItemDimensions(30, 30);
	setPadding(4, 4);
	mComponents = 0;
}
	
wContainer::~wContainer()
{

}

void wContainer::setItemDimensions(int width, int height)
{
	mItemWidth = width;
	mItemHeight = height;
}
	
void wContainer::setPadding(int horizontal, int vertical)
{
	mHorizontalPadding = horizontal;
	mVerticalPadding = vertical;
}

void wContainer::draw()
{
	//wComponent::draw();
}
	
void wContainer::arrangeObject(wComponent* component)
{
	Vector topLeft;
	topLeft.x = getPos().x - getWidth()  /  2;
	topLeft.y = getPos().y - getHeight() /  2;

	Vector newPosition;

	int itemsPerRow = getWidth() / (mItemWidth + mHorizontalPadding);
	int y = mComponents / itemsPerRow;
	int x = mComponents % itemsPerRow;

	component->setPos(topLeft + Vector(((float)x + 0.5) * (mItemWidth + mHorizontalPadding), ((float)y + 0.5) * (mItemHeight + mVerticalPadding)));
	component->setZ(getZ() + 1);

	mComponents++;
	mChildren.push_back(component);
}

void wContainer::clearChildren(WindowHandler* windowHandler)
{
	for(int i = 0; i < mChildren.size(); i++)	
		windowHandler->removeWindow(mChildren[i]->getId());
	
	mChildren.clear();
	mComponents = 0;
}

void wContainer::setNumComponents(int num)
{
	mComponents = num;
}