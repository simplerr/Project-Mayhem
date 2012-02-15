#ifndef MENU_H
#define MENU_H

#include <list>
#include <string>
#include "d3dUtil.h"
#include "Input.h"
#include "Enums.h"
#include "Rect.h"


#include <boost\function.hpp>
#include <boost\bind.hpp>

class Vector;

enum ButtonState
{
	STANDARD,
	PRESSED,
	SELECTED
};

enum NavigationType
{
	MOUSE,
	ARROWKEYS
};

struct MenuItem
{
	Rect itemRect;
	std::string itemName;	
	IDirect3DTexture9* standardTexture;
	IDirect3DTexture9* onSelectTexture;
	ButtonState state;
	bool pressable;
	int ID;
	// soundfile etc..
};

enum MenuType
{
	HOR,
	VER
};

class Menu
{
public:
	Menu(std::string menuName, NavigationType a_navigation, bool useFonts = false, int itemAmount = 2, int a_spacing = 30);
	Menu(std::string menuName, NavigationType a_navigation, MenuType type = HOR, int breakCount = 4, bool useFonts = false, int itemAmount = 2, int a_spacing = 30);
	~Menu();

	void draw(void);
	void update(Vector mousePos);
	void update(float mx, float my);

	void addMenuItem(MenuItem newItem);
	void addMenuItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource = NULL);
	void removeItem(std::string itemName);
	void buildMenu(int itemWidth = 100, int itemHeight = 50);
	void buildMenu2(int itemWidth = 100, int itemHeight = 50);

	void displayCoordinates(void);

	void setMenuBackground(char *textureSource, int x, int y, int width, int height);
	void setPressable(std::string name, bool b);

	boost::function<bool(std::string name)> callback;

	template <class T>
	void connect(bool(T::*_callback)(std::string name), T* _object)	{
		callback = boost::bind(_callback, _object, _1);
	}

private:
	std::list<MenuItem> mMenuItemList;
	std::string mName;
	
	HWND mhMainWnd;

	RECT mBackgroundRect;
	IDirect3DTexture9* mBkgdTexture;

	int mSpacing;
	int mBkgdX, mBkgdY;

	NavigationType mNavigation;
	int mNumbersOfItems;
	int mIdCounter;

	bool mUseFonts;
	int mBreakCount;
	MenuType mType;
};

#endif