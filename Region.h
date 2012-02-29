#pragma once

#include "Object.h"
#include "Trigger.h"


class Rect;

class Region : public Object
{
public:
	Region(float x, float y, int width, int height);
	Region(Rect r);
	~Region();
	void draw();
	void update(float dt);

	void setTrigger(Trigger* trig);
	Trigger* getTrigger()	{return mTrigger;}
private:
	Trigger* mTrigger;
	float mCounter;
};