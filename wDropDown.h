#ifndef WDROPDOWN_H
#define WDROPDOWN_H

#include <boost\function.hpp>
#include <boost\bind.hpp>
#include <vector>
#include "wComponent.h"
#include "Rect.h"

class wDropItem
{
public:
	wDropItem(string value, Rect rect);
	wDropItem(){};
	~wDropItem();

	void draw(D3DCOLOR color);

	Rect mRect;
	string mValue;
};


class wDropDown : public wComponent
{
public:
	wDropDown(float x, float y, int width, int height, wId id, string standard = "choose", string textureSource = "#0xFFFFFFFF");
	~wDropDown();

	void update(float dt);
	void draw();

	void onPress();

	void addItem(wDropItem* item);
	void add(string value);
	void collapse();
	void expand();
	void setActive(bool active);

	string		getValue();
	wDropItem*	getActive();


	// The callback function, ex: bool Editor::messageHandler(wId id, wMessage msg)
	boost::function<bool(wId id, wMessage msg)> callback;

	// Function to connect the callback with the right function
	// Ex: button1->connect(&Editor::messageHandler, this);
	template <class T>
	void connect(bool(T::*_callback)(wId id, wMessage msg), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);		// A magic boost function!
	}
private:
	std::vector<wDropItem*> mItemList;
	wDropItem* mActiveItem;
	Rect mExpandRect;
	float mItemHeight;
	bool mExpanded;
};

#endif