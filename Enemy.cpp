#include <time.h>
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "Enemy.h"
#include "Graphics.h"
#include "Level.h"
#include "Player.h"
#include "Projectile.h"
#include "ItemHandler.h"
#include "Loot.h"
#include "Gold.h"
#include "HealthPotion.h"
#include "EnergyPotion.h"
#include "Scrap.h"
#include "Tile.h"
#include "TileHandler.h"

EnemyData::EnemyData(int w, int h, string n, string texture) 
{
	width = w;
	height = h;
	textureSource = texture;
	name = n;
	//setPolygon();
}

EnemyData::~EnemyData() 
{

}

Enemy::Enemy(float x, float y, EnemyData* type) : Object (x, y, type->width, type->height, ENEMY,  type->textureSource)
{
	mExperience = type->experience;
	mRange = type->range;//ptr
	mHp = type->hp;//ptr
	mSpeed = type->speed;
	mWeaponRate = type->weaponRate;
	mAnimation = new Animation(200, 200, .1f, 3, 3);
	mAnimation->setFrame(0); 
	setClass(type);
	initAI(type->ai_data, Vector(x,y));
	mLostSightTimer = .5;
	mTurns = false;

	string texture = "Data\\imgs\\enemy1.png";
	int z = rand() % 4;
	if(z == 1)
		texture = "Data\\imgs\\enemy2.png";
	if(x == 2)
		texture = "Data\\imgs\\enemy3.png";
	if(x == 3)
		texture = "Data\\imgs\\enemy4.png";

	setTexture(texture);

}

Enemy::~Enemy()
{
	delete ai;
}

void Enemy::damage(float dHP) 
{
	modHP(-dHP);
	ai->setSeenEnemy(true);
	ai->setTarget(getLevel()->getPlayer());
	if(mHp<=0) {
		setAlive(false);

		// Drop loot
		// TODO: Add a drop effect, the loot should fly away from the enemy
		// TODO: Add drop rate for different items
		auto map = gItemHandler->getDataMap();
		int loot = rand() % (map.size()+10);
		int i = 0, distance = 0;
		bool any = false;
		for(auto iter = map.begin(); iter != map.end(); iter++, i++) {
			if(i == loot && iter->second.slot != BAG) {
				float angle = (rand() % 340)/(float)100;
				getLevel()->addObject(new Loot(iter->second.name, getPos().x + distance*cosf(angle), getPos().y + distance*sinf(angle)));
				any = true;
				break;
			}
		}
		if(!any) {
			int x = rand() % 5;
			float angle = (rand() % 340)/(float)100;
			if(x == 0) 
				getLevel()->addObject(new HealthPotion(getPos().x + distance*cosf(angle), getPos().y + distance*sinf(angle), 20));
			else if(x == 1) 
				getLevel()->addObject(new EnergyPotion(getPos().x + distance*cosf(angle), getPos().y + distance*sinf(angle), 20));
		}
		
		int x = rand() % 2;
		if(x == 1) {
			getLevel()->addObject(new Gold(getPos().x, getPos().y+15));
			getLevel()->addObject(new Gold(getPos().x-15, getPos().y));
			getLevel()->addObject(new Gold(getPos().x-15, getPos().y-15));
			getLevel()->addObject(new Gold(getPos().x+15, getPos().y));
		}
		else {
			getLevel()->addObject(new Gold(getPos().x, getPos().y+15));
			getLevel()->addObject(new Gold(getPos().x-15, getPos().y));
		}
	}
}

bool Enemy::handleCollision(Object* collider, MTV* mtv)
{
	if (collider->getOwnerId() != getID() && collider->getType() != LOOT) 
	{
		if(collider->getType() == PROJECTILE)
			damage(dynamic_cast<Projectile*>(collider)->getDamage());
		if(collider->getType() == ENEMY)
			ai->flags.patrol = false;
		if(collider->getType() == STRUCTURE && !mTurns) {
			ai->newTarget(false, getRotation());
		}
		if((collider->getType() == ENEMY || collider->getType() == STRUCTURE))
			return true;
	}
	return false;
}

void Enemies::init() 
{
	//Load enemies
	// Load all different items
	TiXmlDocument doc("Data\\enemies.xml");
	doc.LoadFile();
	srand(time(0));

	// Get the root element
	TiXmlElement* root = doc.FirstChildElement();
	
	// Load items from the file ------ TODO
	for(TiXmlElement* item = root->FirstChildElement("Enemy"); item != NULL; item = item->NextSiblingElement("Enemy"))
	{
		EnemyData* enemyData = new EnemyData();
		enemyData->name = item->Attribute("name") == NULL ? "#NOVALUE" : item->Attribute("name");
		enemyData->hp = item->Attribute("hp") == NULL ? 0 : atoi(item->Attribute("hp"));
		enemyData->range = item->Attribute("range") == NULL ? 0 : atoi(item->Attribute("range"));
		enemyData->textureSource = item->Attribute("texture");
		enemyData->speed = item->Attribute("speed") == NULL ? 0 : atoi(item->Attribute("speed"));
		enemyData->width = atoi(item->Attribute("width"));
		enemyData->height = atoi(item->Attribute("height"));
		enemyData->weaponRate = item->Attribute("weaponRate") == NULL ? 1000 : atoi(item->Attribute("weaponRate"));
		enemyData->experience = item->Attribute("xp") == NULL ? 5 : atoi(item->Attribute("xp"));
		enemyData->ai_data.init();

		data[enemyData->name] = enemyData;
	}
}

Enemies::~Enemies() 
{
	data.clear(); //need delete?
}

EnemyData* Enemies::getData(string searchFor) 
{
	//data.size();
	if(data.empty()) {
		
	}
	else
		mIt= data.find(searchFor);
	//Do check if null
	if(mIt != data.end())
		return mIt->second;
	
	return mIt->second;
}

void Enemy::update(float dt) 
{
	//if(ai->flags.patrol || ai->flags.seen_enemy)
	mAnimation->animate(dt);
	mTime -= dt;
	//Calculate AI
	if(mTime < 0) {
		calcAI(dt);
		mTime = .1;
	}

	//Execute AI
	doAI(dt);
}

void Enemy::draw()
{
	gGraphics->drawTexturedPolygon(getPolygon(), gScrap->shadow);
	gGraphics->drawTexturedPolygon(getPolygon(), getTexture(), &mAnimation->getSourceRect());
}

void Enemy::doAI(float dt)
{
	float v;
	AI_action swiAI = ai->getAction();
	switch (swiAI) 
	{
	case AI_MOVEATTACK:
		mAnimation->resume();
		v = gMath->calculateAngle(getPos(), ai->getObjectTarget());
		setRotation(v);
		if(gMath->distance(getPos(), ai->getObjectTarget())>ai->getRange()) 
		{
			move((cos(v)*getSpeed()*17*dt), (sin(v)*getSpeed()*17*dt));
		}
		else { //Attack
			mAnimation->pause();
			if(mAttackTimer < 0) {
				getLevel()->addProjectile(this, getPos(), ai->getObjectTarget());
				mAttackTimer = (float)(mWeaponRate/100);
			}
			else 
				mAttackTimer-=dt;
		}
		break;
	case AI_RETURN:
		mAnimation->resume();
		v = gMath->calculateAngle(getPos(), ai->getPatrolPos());
		setRotation(v);
		if(gMath->distance(getPos(), ai->getPatrolPos())>10) 
		{
			move((cos(v)*getSpeed()*10*dt), (sin(v)*getSpeed()*10*dt));
		}
		else
			ai->flags.returnToOrigin = false;
		break;
	case AI_PATROL: {
		mAnimation->resume();
		v = gMath->calculateAngle(getPos(), ai->getActionTarget());
		//setRotation(v);
		float diff = v - getRotation();
		
		if(abs(diff) > .1f) {
			mTurns = true;
			rotate((diff/8.0f));
		}
		else {
			if(turns())
				mTurns = false;
			if(gMath->distance(getPos(), ai->getActionTarget())>10) 
			{
				move((cos(v)*getSpeed()*10*dt), (sin(v)*getSpeed()*10*dt));
			}
			else {
				ai->flags.patrol = false;
				mAnimation->pause();	
			}
		}
		break;
					}
	case AI_IDLE:
		mAnimation->pause();
		break;
	default:
		break;
	}
	//Turn
	//Move
	//if, Attack
}

void Enemy::calcAI(float dt)
{
	Vector targetPos = getLevel()->getPlayer()->getPos();
	Vector pos = getPos();
	float v = gMath->calculateAngle(pos, targetPos);
	float vr = ai->getVisionRange();
	float a = gMath->distance(targetPos, pos);
	if(((a<vr) && (a < vr/2 || abs(v-getRotation())< 2*PI/3))) {
		if(!ai->seenEnemy() && doVision()) 
		{
			ai->setSeenEnemy(true);
			ai->setTarget(getLevel()->getPlayer());
		}
		mLostSightTimer = .5;//ai->Patience
	}
	else if (a>v && ai->seenEnemy()) {
		mLostSightTimer-=dt;
	}
	if(mLostSightTimer < 0 && ai->seenEnemy()) 
	{
		ai->setSeenEnemy(false);
		ai->flags.returnToOrigin = true;
		mLostSightTimer = .7;
	}
}

void Enemy::initAI(AIdata data, Vector v) 
{
	ai = new AI(data);
	ai->setPatrolOrigin(v);
}

bool Enemy::doVision()
{
	float d = gMath->distance(getPos(), getLevel()->getPlayer()->getPos());
	float v = gMath->calculateAngle(getPos(), getLevel()->getPlayer()->getPos());
	cPolygon c(getPos());
	c.addPoint(0,2);
	c.addPoint(d,2);
	c.addPoint(d,0);
	gScrap->enemyCheck->setPolygon(c);
	gScrap->enemyCheck->setRotation(v);
	MTV mtv;
	for(int j = 0; j < getLevel()->getTileList()->size(); j++)
		{
			Tile* tile = getLevel()->getTileList()->at(j);

			// Collidable?
			if(!tile->getData()->obstacle)
				continue;
			gScrap->tileObject->setPos(Vector(getLevel()->getOffset().x + tile->getPosition().x, getLevel()->getOffset().y + tile->getPosition().y));
			// Players bounding box
			Rect boundingBox = gScrap->enemyCheck->getBoundingBox();
			gScrap->rect->setPos(Vector(tile->getPosition().x + getLevel()->getOffset().x, tile->getPosition().y + getLevel()->getOffset().y));

			// Check bounding boxes
			if(boundingBox.left > gScrap->rect->right || boundingBox.right < gScrap->rect->left || boundingBox.top > gScrap->rect->bottom || boundingBox.bottom < gScrap->rect->top)
				continue;

			// Get information about the collision
			mtv = checkCollision(gScrap->enemyCheck->getPolygon(), gScrap->tileObject->getPolygon());

			// Move the player out of the tile if there's a collision
			if(mtv.collision)	{
				return false;
			}
		}
	return true;
}