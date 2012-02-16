#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Object.h"

class Projectile : public Object
{
public:
	Projectile(float x, float y, int width, int height, float velocity, std::string textureSource = "Data\\imgs\\standard_box.bmp");
	~Projectile();
	void draw();
	void update(float dt);
	bool handleCollision(Object* collider, MTV* mtv);

	void setVelocity(float velocity)	{mVelocity = velocity;}

	float getVelocity()					{return mVelocity;}

private:
	float mVelocity;
	float mMaxDistance;
	float mTravelled;
};

#endif