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

	void setTrigger(Trigger *trig);
	void initTrigger(Region* owner);
	Trigger* getTrigger()		{return mTrigger;}
	
	void setSelected(bool b)	{mSelected = b;}
private:
	bool mSelected;
	Trigger* mTrigger;
	float mCounter;
};