#include "Projectile.h"
#include "Graphics.h"
#include "Level.h"
#include "FX.h"

Projectile::Projectile(float x, float y, int width, int height, float velocity, std::string textureSource)
	: Object(x, y, width, height, PROJECTILE, textureSource)
{
	setLayer(MIDDLE);
	setVelocity(velocity);
	setLifetime(99999);
	setFollowingPlayer(false);
	mTravelled = 0.0f;
	mMaxDistance = 250;
}

Projectile::~Projectile()
{
	
}

void Projectile::update(float dt)
{
	if(mFollowingPlayer)
		setPos(mOwner->getPos() + mOffset);

	mLifetime -= dt;

	float dx = getVelocity() * cosf(getRotation());
	float dy = getVelocity() * sinf(getRotation());
	
	mTravelled += sqrt(dx*dx + dy*dy);

	move(dx, dy);

	if(mTravelled >= mMaxDistance || mLifetime <= 0)
		setAlive(false);
}

void Projectile::draw()
{
	gGraphics->drawTexturedPolygon(getPolygon(), getTexture());
}

bool Projectile::handleCollision(Object* collider, MTV* mtv)
{
	if (getOwnerId() != collider->getID()) 
	{
		if((collider->getType() == STRUCTURE || collider->getType() == PLAYER || collider->getType() == ENEMY)) {
			setAlive(false);
			FX *f = new FX(getPos().x, getPos().y, 64,64,0.3f,4, "Data\\imgs\\hit_fx.png");
			f->rotate(getRotation()+PI);
			getLevel()->addObject(f);
		}
		if(collider->getType() == ENEMY)
			collider->handleCollision(this, mtv);
		
	}
	return false;
}

void Projectile::setLifetime(float lifetime)
{
	mLifetime = lifetime;
}

void Projectile::setOwner(Object* object)
{
	mOwner = object;
}

void Projectile::setFollowingPlayer(bool following)
{
	mFollowingPlayer = following;
}