#pragma once
#include "Object.h"

class Tri :
	public Object
{
public:
	void spawn();
	void erase();
	void render();
	Tri();
	~Tri();
};