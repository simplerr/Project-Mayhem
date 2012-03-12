#pragma once

#include "wComponent.h"

class wLabel : public wComponent
{
public:
	wLabel(float x, float y, int width, int height, wId id, string label);
	~wLabel();

	void draw();
private:
	string mLabel;
};
