#pragma once
#include "d3dUtil.h"
#include "Vector.h"
#include "Projectile.h"

class Level;
class Player;
struct ProjectileData;

class Weapon
{
public:
	Weapon(Player* player, Level* level);
	virtual	~Weapon();
	virtual void fire(Vector playerPos, float rotation);

	void update(float dt);
	void draw(Vector playerPos, float rotation);
	void setOffset(Vector offset);
	void setCooldown(float cooldown);
	Level* getLevel();
	Player* getPlayer();
	bool isReady();
	void setCounter(float count);
	void setProjectileData(ProjectileData projectileData);
	void setTexture(string source);
	void setVisible(bool visible);
	void setWidth(int width);
	void setHeight(int height);
	bool isVisible();
	ProjectileData getProjectileData();
private:
	IDirect3DTexture9* mTexture;
	ProjectileData mProjectileData;
	Vector mOffset;
	Player* mPlayer;
	Level* mLevel;
	bool mVisible;
	int mWidth, mHeight;
	float mCooldown;
	float mCounter;
};