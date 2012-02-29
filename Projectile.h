#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Object.h"
#include <string>

struct ProjectileData 
{
	std::string texturePath;
	int spread;
	float damage;
	float speed;
	int width;
	int height;
	int range;
	ProjectileData() {
		spread = 10;
		damage = 1;
		speed = 20;
		width = 20;
		height = 7;
		range = 250;
		texturePath = "Data\\imgs\\standard_box.bmp";
	}
};

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
	void setDamage(float dmg)			{mDmg = dmg;}
	float getDamage()					{return mDmg;}
	void setMaxDistance(float distance)	{mMaxDistance = distance;}

private:
	float mDmg;
	float mVelocity;
	float mMaxDistance;
	float mTravelled;
};

#endif