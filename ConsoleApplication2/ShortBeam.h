#pragma once
#include "Projectile.h"

class ShortBeam :
	public Projectile
{
public:
	void spawnProj(int x, int y);
	void erase();
	void render();
	ShortBeam();
	~ShortBeam();
};
