#include "Projectile.h"
#include "Graphics.h"

Projectile::Projectile(float x, float y, int width, int height, float velocity, std::string textureSource)
	: Object(x, y, width, height, PROJECTILE, textureSource)
{
	setLayer(MIDDLE);
	setVelocity(velocity);
	mTravelled = 0.0f;
	mMaxDistance = 250;
}

Projectile::~Projectile()
{
	
}

void Projectile::update(float dt)
{
	float dx = getVelocity() * cosf(getRotation());
	float dy = getVelocity() * sinf(getRotation());
	
	mTravelled += sqrt(dx*dx + dy*dy);

	move(dx, dy);

	if(mTravelled >= mMaxDistance)
		setAlive(false);
}

void Projectile::draw()
{
	gGraphics->drawTexturedPolygon(getPolygon(), getTexture());
}

void Projectile::handleCollision(Object* collider, MTV* mtv)
{
	if(collider->getType() == STRUCTURE || collider->getType() == PLAYER || collider->getType() == ENEMY)
		setAlive(false);
}
