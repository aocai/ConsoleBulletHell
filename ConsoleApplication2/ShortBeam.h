#pragma once
#include "Projectile.h"

class ShortBeam :
	public Projectile
{
public:
	void spawnProj(int x, int y);
	bool updateAndRender();
	void erase();
	void update();
	void render();
	bool intersectsQuadtreeNode();
	bool notOutOfBound();
	bool projInQuadtreeNode(QuadtreeNode *qNode);
	bool collisionTest(QuadtreeNode *qNode);
	ShortBeam();
	~ShortBeam();
};
