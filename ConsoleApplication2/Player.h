#pragma once
#include "Object.h"
#include "QuadtreeNode.h"

class Player :
	public Object
{
public:
	int width, height;
	void spawn();
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
