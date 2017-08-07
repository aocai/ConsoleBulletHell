#pragma once
#include "Projectile.h"

class NormalAtk :
	public Projectile
{
public:
	void erase();
	void render();
	NormalAtk(int, int);
	~NormalAtk();
};

