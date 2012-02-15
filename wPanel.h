#ifndef WPANEL_H
#define WPANEL_H

#include "wComponent.h"

class wPanel : public wComponent
{
public:
	wPanel(float x, float y, int width, int height, wId id, string textureSource = "#0xFFFFFFFF");
	~wPanel();
private:
};

#endif