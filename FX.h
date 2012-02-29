#pragma once
#include "Object.h"
#include "Animation.h"

class FX : public Object {
public:
	FX(float x, float y, float w, float h, float life);
	~FX();

	void draw();
	void update(float dt);

private:
	Animation*	mAnimation;

	float mLife;
	float mCounter;
};