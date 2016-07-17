#pragma once
#include "Object.h"
#include "QuadtreeNode.h"

class Player :
	public Object
{
public:
	void spawn();
	bool updateAndRender();
	void erase();
	void render();
	bool playerInQuadtreeNode(QuadtreeNode *qNode);
	bool collisionDetection(QuadtreeNode *node);
	void spawnShortBeam();
	void spawnNormalAtk();
	void update();
	void ult();
	Player();
	~Player();
};
