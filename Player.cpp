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

Player::Player(float x, float y) : Object (x, y, 40, 40, PLAYER,  "Data\\imgs\\terror.bmp") //change width/heigth/pic
{
	setCooldown(.1f);
	setLayer(MIDDLE);

	mEnergy = mMaxEnergy = 100;
	mHealth = mMaxHealth = 100;
	mInventory = new Inventory();
	mInventory->addItem("Golden Sword");
	mInventory->addItem("Golden Axe");
	mInventory->addItem("Golden Helmet");
	mInventory->addItem("Golden Chest");
	mInventory->addItem("Golden Legs");
	mAnimation = new Animation(64, 54, .2f, 2, 2);
	mAnimation->setFrame(0);
	mCounter = 0.0f;
	mMoveSpeed = 5;
	mGui = new Gui(this);
	mGui->setTexture("Data\\imgs\\gui_bkgd.png");
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
	gGraphics->drawTexturedPolygon(getPolygon(), getTexture(), &mAnimation->getSourceRect());
	mInventory->draw();
	mGui->draw();
}

void Player::handleInput()
{
	// Mouse pressed
	if(gInput->keyPressed(VK_LBUTTON))
	{
		mHealth -= 5;
		mEnergy -= 15;
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
			getLevel()->addProjectile(this, pos, 10);
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
	return false;
}

void Player::attack(/*KEY/WPN used*/)
{

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
	
float Player::getMaxHealth()
{
	return mMaxHealth;
}

float Player::getEnergy()
{
	return mEnergy;
}
	
float Player::getMaxEnergy()
{
	return mMaxEnergy;
}