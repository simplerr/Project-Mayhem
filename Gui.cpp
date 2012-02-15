#include "Graphics.h"
#include "Gui.h"
#include "Player.h"

Gui::Gui(Player* player)
{
	mSlotTexture = gGraphics->loadTexture("Data\\imgs\\skill_slot.png");
	mPlayer = player;
	mPos = Vector(300, 528);
	mWidth = 600;
	mHeight = 90;
}
	
Gui::~Gui()
{
	ReleaseCOM(mSlotTexture);
}

void Gui::update(float dt)
{
	// Check for key presses 1-9
	// Let the player perform the skills
	// mPlayer->skill(num)
}
	
void Gui::draw()
{
	char buffer[256];
	gGraphics->drawRect(mPos.x, mPos.y, mWidth, mHeight, 0xff999999);
	//gGraphics->drawText("Health: ", 150, 500, SMALL_DX);

	Vector hpPos(mPos.x - 200, mPos.y - 10);
	Vector energyPos(hpPos.x, hpPos.y + 25);

	// HP and mana bar bkgd
	gGraphics->drawRect(hpPos.x, hpPos.y, 150, 25, 0xff000000);
	gGraphics->drawRect(energyPos.x, energyPos.y, 150, 25, 0xff000000);

	float hp = mPlayer->getHealth() / mPlayer->getMaxHealth();
	gGraphics->drawRect(hpPos.x - (float)145/2 + 145*hp/2, hpPos.y, 145*hp, 20, 0xffff0000);

	float energy = mPlayer->getEnergy() / mPlayer->getMaxEnergy();
	gGraphics->drawRect(energyPos.x - (float)145/2 + 145*energy/2, energyPos.y, 145*energy, 20, 0xff0000ff);

	// Skill slots
	int x = mPos.x - 60;
	for(int i = 0; i < 7; i++) {
		gGraphics->drawTexture(mSlotTexture, x + i * 50, mPos.y, 40, 40);
	}
}