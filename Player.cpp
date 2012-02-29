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

Player::Player(float x, float y) : Object (x, y, 40, 40, PLAYER,  "Data\\imgs\\spelaren.png") //change width/heigth/pic
{
	setCooldown(.1f);
	setLayer(MIDDLE);

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
	mAnimation = new Animation(64, 54, .2f, 2, 2);
	mAnimation->setFrame(0);
	mCounter = 0.0f;
	mGui = new Gui(this);
	mGui->setTexture("Data\\imgs\\gui_bkgd.png");

	mWeapon = gGraphics->loadTexture("Data\\imgs\\vapen1.png");

	// Experience per level
	mExpPerLevel.push_back(100);
	mExpPerLevel.push_back(250);
	mExpPerLevel.push_back(500);
	mExpPerLevel.push_back(1000);
	mExpPerLevel.push_back(2000);
}
Player::~Player() 
{
	delete mAnimation;
	delete mInventory;
	delete mGui;
}

void Player::update(float dt)
{
	mInventory->update(dt);
	mAnimation->animate(dt);
	mGui->update(dt);
	handleInput();
	mCounter += dt;
}

void Player::draw()
{
	static Vector offset(30, 16);

	gGraphics->drawTexturedPolygon(getPolygon(), getTexture());
	gGraphics->drawTexture(mWeapon, getPos().x + cosf(getRotation())*offset.x - sinf(getRotation())*offset.y, getPos().y + sinf(getRotation())*offset.x + cosf(getRotation())*offset.y, 64, 16, getRotation());
	//gGraphics->drawTexturedPolygon(getPolygon(), getTexture(), &mAnimation->getSourceRect());
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
			mInventory->addItem(loot->getName());
			getLevel()->removeObjectAt(gInput->mousePosition().x, gInput->mousePosition().y);
		}
		else if(object != NULL && object->getType() == GOLD_COIN) {
			Gold* gold = dynamic_cast<Gold*>(object);
			mInventory->addGold(gold->getAmount());
			getLevel()->removeObjectAt(gInput->mousePosition().x, gInput->mousePosition().y);
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

	// Mouse
	Vector mousePos = gInput->mousePosition();
	float angle = gMath->calculateAngle(getPos(), mousePos);

	if(mCounter >= mCooldown)	// :NOTE: && 0
	{
		if(gInput->keyDown(VK_LBUTTON))	{
			Vector pos = gInput->mousePosition();
			getLevel()->addProjectile(this, pos);
			mCounter = 0.0f;
		}
	}

	resetRotation();
	rotate(angle);
	mAngle = angle;
}

bool Player::handleCollision(Object* collider, MTV* mtv)
{
	if(collider->getType() == ENEMY || collider->getType() == STRUCTURE) {
		getLevel()->moveObjects(-mtv->pushX, -mtv->pushY);
	}
	if(collider->getType() == PROJECTILE)
		damage(dynamic_cast<Projectile*>(collider)->getDamage());
	return false;
}

void Player::damage(float dmg)
{
	mHealth-=dmg;
	//CHECK IF DEAD
}

void Player::itemEquipped(Item* item, bool equiped)
{
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

int Player::getHealth()
{
	return mHealth;
}
	
int Player::getMaxHealth()
{
	return mMaxHealth;
}

int Player::getEnergy()
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