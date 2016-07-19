#pragma once
#include "Projectile.h"
#include "QuadtreeNode.h"

class NormalAtk :
	public Projectile
{
public:
	void spawnProj(int x, int y);
	void erase();
	void render();
	NormalAtk();
	~NormalAtk();
};

