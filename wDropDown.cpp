#include "wDropDown.h"
#include "wComponent.h"
#include "Graphics.h"
#include "Input.h"

wDropItem::wDropItem(string value, Rect rect)
{
	mRect = rect;
	mValue = value;
}

wDropItem::~wDropItem()
{

}

void wDropItem::draw(D3DCOLOR color)
{
	gGraphics->drawRect(mRect, color);
	gGraphics->drawText((char*)mValue.c_str(), mRect.left, mRect.top, TextType::SMALL_DX);
}

wDropDown::wDropDown(float x, float y, int width, int height, wId id, string standard, string textureSource)
	: wComponent(x, y, width, height, id)
{
	mExpandRect = Rect(x, y, width, height, 0);
	mExpanded = false;
	mActiveItem = new wDropItem(standard, mExpandRect);
	mItemHeight = height;
}

wDropDown::~wDropDown()
{

}

void wDropDown::update(float dt)
{
	
}

void wDropDown::draw()
{
	if(mActiveItem != NULL)	{
		gGraphics->drawRect(mExpandRect, D3DCOLOR_ARGB(255, 255, 50, 100));
		gGraphics->drawText((char*)mActiveItem->mValue.c_str(), mExpandRect.left, mExpandRect.top, TextType::SMALL_DX);
	}
	
	if(mExpanded == true)
	{
		bool color = false;
		for(int i = 0; i < mItemList.size(); i++) {	
			mItemList[i]->draw(D3DCOLOR_ARGB(255, 255, 226, 7));
		}
	}
}

bool wDropDown::onPress()
{
	// The expand rect was pressed
	if(!mExpanded)
	{
		if(gInput->keyPressed(VK_LBUTTON) &&  gMath->pointInsideRect(gInput->mousePosition(), mExpandRect));	{
			expand();
		}
	}
	else if(mExpanded)
	{
		if(gInput->keyPressed(VK_LBUTTON))
		{
			if(gMath->pointInsideRect(gInput->mousePosition(), mExpandRect))	{
				collapse();
			}		
			else
			{
				for(int i = 0; i < mItemList.size(); i++)	{
					if(gMath->pointInsideRect(gInput->mousePosition(), mItemList[i]->mRect))	{
						if(mActiveItem != mItemList[i])
							callback(getId(), mItemList[i]->mValue);

						mActiveItem = mItemList[i];
						collapse();
					}
				}
			}
		}
	}

	return false;
}

void wDropDown::setActive(bool active)
{	
	if(!active && mExpanded)
		collapse();

	wComponent::setActive(active);
}

void wDropDown::addItem(wDropItem* item)
{
	mItemList.push_back(item);
}

void wDropDown::expand()
{
	mExpanded = true;
	setHeight((mItemList.size()+1) * mItemHeight);
	setPos(Vector(getPos().x, getPos().y + getHeight()/2 - mItemHeight/2));
}

void wDropDown::collapse()
{
	setPos(Vector(getPos().x, getPos().y - getHeight()/2 + mItemHeight/2));
	setHeight(mItemHeight);
	mExpanded = false;
}

void wDropDown::add(string value)
{
	int x = getPos().x;
	int y = getPos().y + (mItemList.size() + 1)*getHeight();
	int width = getWidth();
	int height = getHeight();

	Rect tmp = Rect(x-width/2, x+width/2, y-height/2, y+height/2);
	
	wDropItem* item = new wDropItem(value, tmp);
	addItem(item);
}

wDropItem* wDropDown::getActive()
{
	return mActiveItem;
}

string wDropDown::getValue()
{
	return mActiveItem->mValue;
}