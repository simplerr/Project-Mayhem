#include "Graphics.h"
#include "Gui.h"
#include "Player.h"
#include "Input.h"
#include "Skill.h"

Gui::Gui(Player* player) : Container(300, 700, 600, 90)
{
	// Load the slot texture
	mSlotTexture = gGraphics->loadTexture("Data\\imgs\\skill_slot.png");

	// Set the player
	mPlayer = player;

	// Init skill slots
	for(int i = 0; i < 6; i++) {
		addSlot(getPosition().x + i*50, getPosition().y, SKILL);
	}

	// Add some test skills
	addSkill(new Skill());
	Skill* skill = new Skill();
	skill->setTexture("Data\\imgs\\fire_bolt.png");
	addSkill(skill);

	// Set visible
	show();
}
	
Gui::~Gui()
{
	ReleaseCOM(mSlotTexture);
}

void Gui::update(float dt)
{
	// Standard update
	Container::update(dt);

	// Check if a slot with a skill in it was pressed
	// Keys 1-6
	// TODO: Check for right press with the mouse
	for(int i = 0; i < mSlotList.size(); i++) {
		if(gInput->keyPressed(49 + i) && mSlotList[i].taken)
			((Skill*)mSlotList[i].item)->performSkill(mPlayer);
	}
}
	
void Gui::draw()
{
	// Standard draw
	Container::draw();

	// Position of hp and energy bars
	Vector hpPos(getPosition().x - 200, getPosition().y - 10);
	Vector energyPos(hpPos.x, hpPos.y + 25);

	// Their bkgds
	gGraphics->drawRect(hpPos.x, hpPos.y, 150, 25, 0xff000000);
	gGraphics->drawRect(energyPos.x, energyPos.y, 150, 25, 0xff000000);

	// Draw the hp and energy bars
	float filledPercent = mPlayer->getHealth() / mPlayer->getMaxHealth();
	gGraphics->drawRect(hpPos.x - (float)145/2 + 145*filledPercent/2, hpPos.y, 145*filledPercent, 20, 0xffff0000);

	filledPercent = mPlayer->getEnergy() / mPlayer->getMaxEnergy();
	gGraphics->drawRect(energyPos.x - (float)145/2 + 145*filledPercent/2, energyPos.y, 145*filledPercent, 20, 0xff0000ff);
}

void Gui::addSkill(Skill* skill)
{
	// Adds a new skill to the first free slot
	for(int i = 0; i < mSlotList.size(); i++) 
	{
		if(!mSlotList[i].taken) {
			mSlotList[i].item = skill;
			mSlotList[i].taken = true;
			break;
		}
	}
}