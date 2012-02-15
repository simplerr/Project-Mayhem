#pragma once
#include <string>
#include "Object.h"
using namespace std;

class Loot : public Object
{
public:
	Loot(string name, int x, int y);
	~Loot();

	string getName();
private:
	string mName;
};