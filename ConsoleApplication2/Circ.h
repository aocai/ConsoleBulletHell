#pragma once
#include "Object.h"

class Circ :
	public Object
{
public:
	void spawn();
	void erase();
	void render();
	Circ();
	~Circ();
};
