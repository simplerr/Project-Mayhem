#pragma once
#include <string>
#include "Object.h"
using namespace std;

class Loot : public Object
{
public:
	Loot(string name, int x, int y);
	~Loot();

	void update(float dt);
	virtual void draw();
	void setLifetime(float lifetime);
	virtual bool handleCollision(Object* collider, MTV* mtv);

	string getName();
private:
	float mCounter;
	string mName;
};