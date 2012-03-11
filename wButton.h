#ifndef WINBUTTON_H
#define WINBUTTON_H

#include <string>
#include <boost\function.hpp>
#include <boost\bind.hpp>
#include "wComponent.h"
#include "d3dUtil.h"

using namespace std;

class wButton : public wComponent
{
public:
	wButton(int x, int y, int width, int height, string label, wId id = WID_STANDARD, string textureSource = "Data\\imgs\\orange_button.bmp");
	~wButton();

	void draw();
	bool onPress();
	void onHoover();
	
	// The callback function, ex: bool Editor::messageHandler(wId id, wMessage msg)
	boost::function<bool(wId id, wMessage msg)> callback;

	// Function to connect the callback with the right function
	// Ex: button1->connect(&Editor::messageHandler, this);
	template <class T>
	void connect(bool(T::*_callback)(wId id, wMessage msg), T* _object)	{
		callback = boost::bind(_callback, _object, _1, _2);		// A magic boost function!
	}
private:
	string mLabel;
	IDirect3DTexture9* mBkgdTexture;
	IDirect3DTexture9* mTexture;
};

#endif