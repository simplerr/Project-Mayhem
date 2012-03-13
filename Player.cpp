#include "Player.h"
#include "Level.h"
#include "Input.h"
#include "Vector.h"
#include "Math.h"
#include "Graphics.h"
#include "Projectile.h"
#include "Inventory.h"
#include "Loot.h"
#include "Gold.h"
#include "Gui.h"
#include "Shotgun.h"
#include "Rifle.h"
#include "PlayState.h"
#include "GameOver.h"
#include "Blink.h"
#include "Wrath.h"
#include "SolarBolts.h"
#include "Mine.h"
#include "StatusText.h"
#include "HealthPotion.h"
#include "EnergyPotion.h"
#include "Knife.h"	

Player::Player(float x, float y) : Object (x, y, 40, 40, PLAYER,  "Data\\imgs\\player.png") //change width/heigth/pic
{
	setCooldown(.1f);
	setLayer(MIDDLE);

	// No weapon to start with -TODODOODO
	mWeapon = NULL;

	mEnergy = mMaxEnergy = 100;
	mHealth = mMaxHealth = 100;
	mLevel = 1;
	mExperience = 0;
	mArmor = mBaseArmor = 3;
	mMoveSpeed = 5;
	mInventory = new Inventory();
	mInventory->setPlayer(this);
	mInventory->addItem("Golden Sword");
	mInventory->addItem("Golden Axe");
	mInventory->addItem("Golden Helmet");
	mInventory->addItem("Golden Chest");
	mInventory->addItem("Golden Legs");
	
	mAnimation = new Animation(200, 200, .1f, 3, 3);
	mAnimation->setFrame(0);
	mCounter = 0.0f;
	mGui = new Gui(this);
	mGui->setTexture("Data\\imgs\\gui_bkgd.png");
	mStatusText = new StatusText("Data\\imgs\\level_up.png", SCREEN_WIDTH/2, 200, 400, 50);

	

	// Experience per level
	mExpPerLevel.push_back(50);
	mExpPerLevel.push_back(100);
	mExpPerLevel.push_back(200);
	mExpPerLevel.push_back(500);
	mExpPerLevel.push_back(1000);
}
Player::~Player() 
{
	delete mAnimation;
	delete mInventory;
	delete mGui;
	delete mStatusText;

	if(mWeapon != NULL)
		delete mWeapon;
}

void Player::init()
{
	mInventory->addItem("Knife", WEAPON);
}

void Player::update(float dt)
{
	mInventory->update(dt);
	mAnimation->animate(dt);
	mGui->update(dt);
	mStatusText->update(dt);

	if(mWeapon != NULL)
		mWeapon->update(dt);

	handleInput();
	mCounter += dt;
}

void Player::draw()
{
	gGraphics->drawTexturedPolygon(getPolygon(), getTexture(), &mAnimation->getSourceRect());
	if(mWeapon != NULL)
		mWeapon->draw(getPos(), getRotation());

	mStatusText->draw();
	mInventory->draw();
	mGui->draw();
}

void Player::handleInput()
{
	// Mouse pressed
	if(gInput->keyPressed(VK_LBUTTON))
	{
		Object* object = getLevel()->getObjectAt(gInput->mousePosition());
		if(object != NULL && object->getType() == LOOT) {
			Loot* loot = dynamic_cast<Loot*>(object);
			if(loot->getName() == "Health potion") 
				setHealth(getHealth() + dynamic_cast<HealthPotion*>(object)->getHp());	
			else if(loot->getName() == "Energy potion") 
				setEnergy(getEnergy() + dynamic_cast<EnergyPotion*>(object)->getEnergy());
			else if(loot->getName() == "Gold")
				mInventory->addGold(dynamic_cast<Gold*>(object)->getAmount());
			else
				mInventory->addItem(loot->getName());
		
			getLevel()->removeObject(loot);
		}
	}

	if(!gInput->keyDown('W') && !gInput->keyDown('S') && !gInput->keyDown('A') && !gInput->keyDown('D') && !mAnimation->isPaused()) {
		mAnimation->setFrame(1);
		mAnimation->pause();
	}
	else if(mAnimation->isPaused() && gInput->keyDown('W') || gInput->keyDown('S') || gInput->keyDown('A') || gInput->keyDown('D'))
		mAnimation->resume();

	if(gInput->keyDown('W'))	
		getLevel()->moveObjects(0, mMoveSpeed);
	else if(gInput->keyDown('S'))	
		getLevel()->moveObjects(0, -mMoveSpeed);
	if(gInput->keyDown('A'))	
		getLevel()->moveObjects(mMoveSpeed, 0);
	else if(gInput->keyDown('D'))	
		getLevel()->moveObjects(-mMoveSpeed, 0);

	if(gInput->keyDown(VK_LBUTTON))	{
		if(mWeapon != NULL && mWeapon->isReady())
			mWeapon->fire(getPos(), getRotation());
	}

	// Look at the mouse
	float angle = gMath->calculateAngle(getPos(), gInput->mousePosition());

	resetRotation();
	rotate(angle);
	mAngle = angle;
}

bool Player::handleCollision(Object* collider, MTV* mtv)
{
	if(collider->getType() == STRUCTURE)
		getLevel()->moveObjects(-mtv->pushX, -mtv->pushY);
	else if(collider->getType() == ENEMY) {
		getLevel()->moveObjects(mtv->pushX, mtv->pushY);
		move(-mtv->pushX, -mtv->pushY);
	}

	if(collider->getType() == PROJECTILE)
		damage(dynamic_cast<Projectile*>(collider)->getDamage());
	
	return false;
}

void Player::damage(float dmg)
{
	mHealth-=dmg;
}

void Player::itemEquipped(Item* item, bool equiped)
{
	// Equipped a weapon
	if(item->getSlotId() == WEAPON)
	{
		if(mWeapon != NULL) {
			delete mWeapon;
			mWeapon = NULL;
		}

		// Weapon equipped?
		if(equiped)	{
			if(item->getData().name == "Golden Sword")
				mWeapon = new Shotgun(this, getLevel());
			else if(item->getData().name == "Golden Axe") {
				mWeapon = new Rifle(this, getLevel());
				mWeapon->setCooldown(0.1f);
			}
			else if(item->getData().name == "Knife")
				mWeapon = new Knife(this, getLevel());
		}
	}

	int x = equiped ? 1 : -1;

	ItemData data = item->getData();
	mArmor += data.armor*x;
	mMaxHealth += data.health*x;
	mMoveSpeed += data.moveSpeed*x;
	mMaxEnergy += data.energy*x;
}

void Player::attack(/*KEY/WPN used*/)
{

}

void Player::addExperience(int experience)
{
	if(getExperience() + experience >= mExpPerLevel[mLevel-1] && mLevel != mExpPerLevel.size()) {
		mExperience = getExperience() + experience - mExpPerLevel[mLevel-1];
		mLevel++;

		if(mLevel == 2)
			mGui->addSkill(new Blink());
		else if(mLevel == 3)
			mGui->addSkill(new Wrath());
		else if(mLevel == 4)
			mGui->addSkill(new Mine());
		else if(mLevel == 5)
			mGui->addSkill(new SolarBolts());

		mStatusText->show(2.0f);
	}
	else
		mExperience += experience;
}

void Player::addGold(int amount)
{
	mInventory->addGold(amount);
}

void Player::move(float dx, float dy) 
{
	Object::move(dx, dy);
}

void Player::setCooldown(float cooldown)
{
	mCooldown = cooldown;
}

float Player::getHealth()
{
	return mHealth;
}
	
int Player::getMaxHealth()
{
	return mMaxHealth;
}

float Player::getEnergy()
{
	return mEnergy;
}
	
int Player::getMaxEnergy()
{
	return mMaxEnergy;
}

float Player::getMoveSpeed()
{
	return mMoveSpeed;
}
	
int Player::getArmor()
{
	return mArmor;
}

int Player::getExperience()
{
	return mExperience;
}

int Player::getCharacterLevel()
{
	return mLevel;
}

int Player::getLevelExp()
{
	return mExpPerLevel[mLevel-1];
}

bool Player::inInventory()
{
	return mInventory->getVisible();
}

void Player::setHealth(int hp)
{
	mHealth = hp > mMaxHealth ? mMaxHealth : hp;
}
	
void Player::setEnergy(int energy)
{
	mEnergy = energy > mMaxEnergy ? mMaxEnergy : energy;
}