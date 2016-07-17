#pragma once
#include "Projectile.h"
#include "QuadtreeNode.h"

class NormalAtk :
	public Projectile
{
public:
	void spawnProj(int x, int y);
	bool updateAndRender();
	void erase();
	void update();
	void render();
	bool notOutOfBound();
	bool intersectsQuadtreeNode();
	bool projInQuadtreeNode(QuadtreeNode *qNode);
	bool collisionTest(QuadtreeNode *qNode);
	NormalAtk();
	~NormalAtk();
};

