#pragma once
#include "Object.h"

class QuadtreeNode;

class Player :
	public Object
{
private:
	Player() {}
	Player(Player const&);
	void operator=(Player const&);
public:
	int width, height;
	static Player& getInstance();
	void spawn();
	void erase();
	void render();
	bool playerInQuadtreeNode(QuadtreeNode *qNode);
	void collisionDetection(QuadtreeNode *node);
	void spawnShortBeam();
	void spawnNormalAtk();
	void update();
	void ult();
	~Player();
};