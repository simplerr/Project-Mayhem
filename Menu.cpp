#include "Menu.h"
#include "Graphics.h"
#include "Enums.h"
#include "Vector.h"
#include "Rect.h"

//#include "Sound.h"


//extern Sound* gSound;

Menu::Menu(std::string menuName, NavigationType a_navigation, bool useFonts, int itemAmount, int a_spacing)
{
	mName = menuName;
	
	// Space between buttons
	mSpacing = a_spacing;
	mNavigation = a_navigation;

	mNumbersOfItems = itemAmount;
	mUseFonts = useFonts;
}

Menu::Menu(std::string menuName, NavigationType a_navigation, MenuType type, int breakCount, bool useFonts, int itemAmount, int a_spacing)
{
	mName = menuName;
	mSpacing = a_spacing;
	mNavigation = a_navigation;
	mNumbersOfItems = itemAmount;
	mUseFonts = useFonts;
	mType = type;
	mBreakCount = breakCount;
}

Menu::~Menu()
{
	ReleaseCOM(mBkgdTexture);
	
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{	
		ReleaseCOM(i->standardTexture);
		ReleaseCOM(i->onSelectTexture);
		i++;
	}
}

void Menu::setMenuBackground(char *textureSource, int x, int y, int width, int height)
{
	// :NOTE: mBackgroundRect is used for a lot more than just the backround texture,
	// which it shouldn't
	if(textureSource != "none")
		mBkgdTexture = gGraphics->loadTexture(textureSource);
	else
		mBkgdTexture = NULL;

	mBackgroundRect.left = x-(width/2);
	mBackgroundRect.right = x+(width/2);
	mBackgroundRect.top = y-(height/2);
	mBackgroundRect.bottom = y+(height/2);

	mBkgdX = x;
	mBkgdY = y;

	mIdCounter = 0;
}

void Menu::addMenuItem(MenuItem newItem)
{
	// :NOTE: Don't know if the item get a proper ID this way
	mMenuItemList.push_back(newItem);
}

void Menu::addMenuItem(std::string itemName, char *standardTextureSource, char *onSelectTextureSource)
{
	static int newID = 0;
	MenuItem tempMenuItem;
	tempMenuItem.itemName = itemName;
	tempMenuItem.state = STANDARD;
	tempMenuItem.standardTexture = gGraphics->loadTexture(standardTextureSource);

	if(onSelectTextureSource == NULL)
		tempMenuItem.onSelectTexture = gGraphics->loadTexture(standardTextureSource);
	else
		tempMenuItem.onSelectTexture = gGraphics->loadTexture(onSelectTextureSource);

	tempMenuItem.ID = newID;
	tempMenuItem.pressable = true;

	newID++;
	mMenuItemList.push_back(tempMenuItem);
}

void Menu::buildMenu(int itemWidth, int itemHeight)
{
	int menuSize = mMenuItemList.size();
	int itemNumber = menuSize/2;
	float isfloat = (float)menuSize/2;

	int width, height, centerX, centerY, diffY;
	width = mBackgroundRect.right - mBackgroundRect.left;
	height = mBackgroundRect.bottom - mBackgroundRect.top;
	centerX = mBackgroundRect.left + (width/2);

	// Handle uneven amount of menuItems
	if((int)isfloat != isfloat)
	{
		// Decimal -> uneven
		std::list<MenuItem>::iterator i = mMenuItemList.begin();	
		while( i != mMenuItemList.end())
		{	 
			if(itemNumber != 0)
			{				
				// Distance from middle in Y
				diffY = itemNumber*(mSpacing +itemHeight);
				// menuItems Y coord
				centerY = mBkgdY - diffY;			
			}
			else if(itemNumber == 0)	{	// :NOTE: behövs inte, är itemNumber = 0 så är diffY = 0!
				// x and y = bkgds x and y
				centerY = mBkgdY;
			}

			i->itemRect.left = centerX-(itemWidth/2);
			i->itemRect.right = centerX+(itemWidth/2);
			i->itemRect.top = centerY-(itemHeight/2);
			i->itemRect.bottom = centerY+(itemHeight/2); 

			i++;
			itemNumber--;
		}		
	}	
	// hantera jämna tal menu items
	else
	{
		std::list<MenuItem>::iterator i = mMenuItemList.begin();	
		while( i != mMenuItemList.end())
		{	 
			if(itemNumber == 0)					
				itemNumber--;	

			if(itemNumber > 0)
				diffY = (itemNumber * (mSpacing + itemHeight)) - 0.5*(mSpacing + itemHeight);
			else if(itemNumber < 0)
				diffY = (itemNumber * (mSpacing + itemHeight)) + 0.5*(mSpacing + itemHeight);

			centerY = mBkgdY - diffY;
				
			i->itemRect.left = centerX-(itemWidth/2);
			i->itemRect.right = centerX+(itemWidth/2);
			i->itemRect.top = centerY-(itemHeight/2);
			i->itemRect.bottom = centerY+(itemHeight/2); 

			i++;
			itemNumber--;
		}
	}
}

void Menu::draw(void)
{	
	//if(backgroundTexture != NULL)
		//gGraphics->drawTexture(backgroundTexture, backgroundRect, 0xFFFFFFFF, 0);

	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{	
		if(i->state == STANDARD)
			gGraphics->drawTexture(i->standardTexture, i->itemRect, 0);
		else if(i->state = SELECTED)
			gGraphics->drawTexture(i->onSelectTexture, i->itemRect, 0);

		if(mUseFonts)
			gGraphics->drawText((char*)i->itemName.c_str(), i->itemRect.left + 5, i->itemRect.top + (i->itemRect.bottom - i->itemRect.top)/2-10);
		i++;
	}	
}

void Menu::update(Vector mousePos)
{
	if(mNavigation == MOUSE)
	{
		std::list<MenuItem>::iterator i = mMenuItemList.begin();
		while( i != mMenuItemList.end())
		{
			// mouse is inside
			if(mousePos.x < i->itemRect.right && mousePos.x > i->itemRect.left && mousePos.y < i->itemRect.bottom && mousePos.y > i->itemRect.top)
			{
				i->state = SELECTED;
				if(gInput->keyPressed(VK_LBUTTON) && i->pressable)	{
					//gSound->playEffect("misc\\sound\\menu_click.wav");
					if(!callback(i->itemName))
						break;
				}		
			}
			else
				i->state = STANDARD;

			i++;
		}
	}
	else if(mNavigation == ARROWKEYS)
	{		
		// updatera idCounter				
		if(gInput->keyPressed('W')){
			mIdCounter--;
		}
		else if(gInput->keyPressed('S'))	{
			mIdCounter++;
		}
	
		// kolla om man är utanför menyn
		if(mIdCounter < 0)
			mIdCounter = mNumbersOfItems-1;	// tänk 0-X
		else if(mIdCounter > mNumbersOfItems-1)
			mIdCounter = 0;			

		std::list<MenuItem>::iterator i = mMenuItemList.begin();
		while( i != mMenuItemList.end())
		{								
			if(i->ID == mIdCounter){				
				i->state = SELECTED;
			}
			else
				i->state = STANDARD;

			// a item was pressed
			if(gInput->keyPressed(VK_RETURN))
			{
				if(i->ID == mIdCounter && i->pressable)	
				{
					if(!callback(i->itemName))
						break;
				}					
			}
			i++;
		}
	}

}

void Menu::buildMenu2(int itemWidth, int itemHeight)
{
	/* stuff neeeded */
	int		menuSize;
	int		widthInItems;
	int		heightInItems;
	int		loopX;
	int		loopY;
	int		dX;
	int		dY;
	int		curX;
	int		curY;
	int		resX, resY;
	int		itemNumber;

	/* number of items */
	menuSize = mMenuItemList.size();

	/* find out width and height counted in items */
	widthInItems = menuSize % mBreakCount;

	/*
	*  this is neccessary to get a correct height
	*  has to do with integers rounding down
	*  will only occur when there's an uneven amount of menu items
	*  adds mBreakCount since adding 1 isn't suffice with large menus
	*/
	if(menuSize % mBreakCount != 0)	{
		heightInItems = (menuSize + mBreakCount) / mBreakCount;
		if(heightInItems != 1)
			widthInItems = mBreakCount;
	}
	else	{
		heightInItems = (menuSize) / mBreakCount;
		if(heightInItems != 0)
			widthInItems = mBreakCount;
	}

	if(menuSize == 1)	{
		widthInItems = 0;
		heightInItems = 0;
	}

	/* if the first row is filled, then we now that the width = break count */
	//if(heightInItems != 0)	// (heightInItems != 1)
	//	widthInItems = mBreakCount;

	itemNumber = 0;

	/* loopa through all items */
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{
		loopX = itemNumber %  mBreakCount;
		loopY = itemNumber /  mBreakCount;

		/* even amount of items in the widest x row */
		if(widthInItems % 2 == 0 && widthInItems != 0)	{
			dX = (loopX * (mSpacing + itemWidth));
			dX -= 0.5*(mSpacing + itemWidth);

			if(menuSize == 1)
				dX = 0;

			resX = mBkgdX - dX - widthInItems*(mSpacing + itemWidth)/2 ;//+ (spacing + itemWidth)/2;
		}
		/* uneven */
		else	{
			dX = loopX*(mSpacing +itemWidth);

			if(menuSize == 1)
				resX = mBkgdX;
			else
				resX = mBkgdX - dX - widthInItems*(mSpacing + itemWidth)/2 + (mSpacing + itemWidth)/2;;
		}

		/* even amount of items in the widest y row */
		if(heightInItems % 2 == 0 && heightInItems != 0)	{
			dY = (loopY * (mSpacing + itemHeight));
			dY -= 0.5*(mSpacing + itemHeight);	
			
			if(menuSize == 1)
				dY = 0;

			resY = mBkgdY - dY - heightInItems*(mSpacing + itemHeight)/2;
		}
		/* uneven */
		else	{		
				dY = loopY*(mSpacing +itemHeight);

				if(menuSize == 1)
					resY = mBkgdY;
				else
					resY = mBkgdY - dY - heightInItems*(mSpacing + itemHeight)/2 + (mSpacing + itemHeight)/2;
		}

		/* center the menu items */
		//centerX -= 

		i->itemRect.left = resX-(itemWidth/2);
		i->itemRect.right = resX+(itemWidth/2);
		i->itemRect.top = resY-(itemHeight/2);
		i->itemRect.bottom = resY+(itemHeight/2); 

		itemNumber--;
		i++;
	}
}

void Menu::setPressable(std::string name, bool b)
{
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{
		if(i->itemName == name)
			i->pressable = b;
		i++;
	}
}

void Menu::removeItem(std::string itemName)
{	
	std::list<MenuItem>::iterator i = mMenuItemList.begin();
	while( i != mMenuItemList.end())
	{
		if(i->itemName == itemName)	{
			mMenuItemList.erase(i);
			buildMenu2();
			return;
		}
		i++;
	}
}

void Menu::update(float mx, float my)
{
	//updateSelectedItem(mousePos);
	if(mNavigation == MOUSE)
	{
		std::list<MenuItem>::iterator i = mMenuItemList.begin();
		while( i != mMenuItemList.end())
		{
			// mouse is inside
			if(mx < i->itemRect.right && mx > i->itemRect.left && my < i->itemRect.bottom && my > i->itemRect.top)
			{
				i->state = SELECTED;
				if(gInput->keyPressed(VK_LBUTTON) && i->pressable)	{
					//gSound->playEffect("misc\\sound\\menu_click.wav");
					if(!callback(i->itemName))
						break;
				}		
			}
			else
				i->state = STANDARD;

			i++;
		}
	}
	else if(mNavigation == ARROWKEYS)
	{		
		// updatera idCounter				
		if(gInput->keyPressed('W')){
			mIdCounter--;
		}
		else if(gInput->keyPressed('S'))	{
			mIdCounter++;
		}
	
		// kolla om man är utanför menyn
		if(mIdCounter < 0)
			mIdCounter = mNumbersOfItems-1;	// tänk 0-X
		else if(mIdCounter > mNumbersOfItems-1)
			mIdCounter = 0;			

		std::list<MenuItem>::iterator i = mMenuItemList.begin();
		while( i != mMenuItemList.end())
		{								
			if(i->ID == mIdCounter){				
				i->state = SELECTED;
			}
			else
				i->state = STANDARD;

			// a item was pressed
			if(gInput->keyPressed(VK_RETURN))
			{
				if(i->ID == mIdCounter && i->pressable)	
				{
					if(!callback(i->itemName))
						break;
				}					
			}

			i++;
		}
	}
}