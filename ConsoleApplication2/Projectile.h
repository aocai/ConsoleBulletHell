#pragma once
#include "Object.h"
#include "QuadtreeNode.h"

class Projectile
{
public:
	int posX; //bottom left
	int posY; //bottom left
	int width;
	int height;
	int speedX; //right = positive
	int speedY; //down = positive
	bool first; //just initialized?
	virtual void spawnProj(int x, int y);
	virtual bool updateAndRender();
	virtual void erase();
	virtual void update();
	virtual void render();
	virtual bool notOutOfBound();
	virtual bool projInQuadtreeNode(QuadtreeNode *qNode);
	virtual bool collisionTest(QuadtreeNode *qNode);
	Projectile();
	~Projectile();
};
