#pragma once
#include "Object.h"

class Circ :
	public Object
{
public:
	void spawn();
	bool updateAndRender();
	void erase();
	void render();
	void update();
	bool notOutOfBound();
	Circ();
	~Circ();
};
