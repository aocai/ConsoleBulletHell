#pragma once
#include "Projectile.h"

class ShortBeam :
	public Projectile
{
public:
	void erase();
	void render();
	ShortBeam(int, int);
	~ShortBeam();
};
