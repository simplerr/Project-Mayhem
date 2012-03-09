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
	setClass(type);
	initAI(type->ai_data, Vector(x,y));
	mLostSightTimer = 1;
}

Enemy::~Enemy()
{
	delete ai;
}

void Enemy::damage(float dHP) 
{
	modHP(-dHP);
	if(mHp<=0) {
		setAlive(false);

		// Drop loot
		// TODO: Add a drop effect, the loot should fly away from the enemy
		// TODO: Add drop rate for different items
		auto map = gItemHandler->getDataMap();
		int loot = rand() % (map.size()+4);
		int i = 0, distance = 0;
		for(auto iter = map.begin(); iter != map.end(); iter++, i++) {
			if(i == loot) {
				float angle = (rand() % 340)/(float)100;
				getLevel()->addObject(new Loot(iter->second.name, getPos().x + distance*cosf(angle), getPos().y + distance*sinf(angle)));
			}
		}

		getLevel()->addObject(new Gold(getPos().x, getPos().y+15));
		getLevel()->addObject(new Gold(getPos().x-15, getPos().y));
		getLevel()->addObject(new Gold(getPos().x-15, getPos().y-15));
		getLevel()->addObject(new Gold(getPos().x+15, getPos().y));
	}
}

bool Enemy::handleCollision(Object* collider, MTV* mtv)
{
	if (collider->getOwnerId() != getID()) 
	{
		if(collider->getType() == PROJECTILE)
			damage(dynamic_cast<Projectile*>(collider)->getDamage());
		if(collider->getType() == ENEMY)
			ai->flags.patrol = false;
		if(collider->getType() == TILE)
			ai->flags.patrol = false;
		if((collider->getType() == ENEMY || collider->getType() == STRUCTURE))
			return true;
	}
	return false;
}

void Enemies::init() 
{
	//Load enemies
	// Load all different items
	TiXmlDocument doc("enemies.xml");
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
	gGraphics->drawTexturedPolygon(getPolygon(), getTexture());
}

void Enemy::doAI(float dt)
{
	float v;
	AI_action swiAI = ai->getAction();
	switch (swiAI) 
	{
	case AI_MOVEATTACK:
		v = gMath->calculateAngle(getPos(), ai->getObjectTarget());
		if(gMath->distance(getPos(), ai->getObjectTarget())>ai->getRange()) 
		{
			move((cos(v)*getSpeed()*17*dt), (sin(v)*getSpeed()*17*dt));
		}
		else { //Attack
			if(mAttackTimer < 0) {
				getLevel()->addProjectile(this, getPos(), ai->getObjectTarget());
				mAttackTimer = (float)(mWeaponRate/100);
			}
			else 
				mAttackTimer-=dt;
		}
		break;
	case AI_RETURN:
		v = gMath->calculateAngle(getPos(), ai->getPatrolPos());
		if(gMath->distance(getPos(), ai->getPatrolPos())>10) 
		{
			move((cos(v)*getSpeed()*10*dt), (sin(v)*getSpeed()*10*dt));
		}
		else
			ai->flags.returnToOrigin = false;
		break;
	case AI_PATROL:
		v = gMath->calculateAngle(getPos(), ai->getActionTarget());
		if(gMath->distance(getPos(), ai->getActionTarget())>10) 
		{
			move((cos(v)*getSpeed()*10*dt), (sin(v)*getSpeed()*10*dt));
		}
		else
			ai->flags.patrol = false;
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
	float v = ai->getVisionRange();
	float a = gMath->distance(targetPos, pos);
	if(a<v) {
		if(!ai->seenEnemy()) 
		{
			ai->setSeenEnemy(true);
			ai->setTarget(getLevel()->getPlayer());
		}
		mLostSightTimer = .7;//ai->Patience
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