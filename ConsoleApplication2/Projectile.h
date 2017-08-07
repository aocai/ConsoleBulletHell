#pragma once

class Object;
class QuadtreeNode;

class Projectile
{
public:
	int minX, minY;
	int maxX, maxY;
	int width;
	int height;
	int speedX; //right = positive
	int speedY; //down = positive
	bool first;
	bool collision;
	virtual void spawnProj(int x, int y);
	virtual void erase();
	virtual void update();
	virtual void render();
	virtual bool notOutOfBound();
	virtual bool projInQuadtreeNode(QuadtreeNode *qNode);
	virtual void collisionTest(QuadtreeNode *qNode);
	Projectile();
	virtual ~Projectile();
};
