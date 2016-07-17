#pragma once
#include "Object.h"

class Tri :
	public Object
{
public:
	void spawn();
	bool updateAndRender();
	void erase();
	void render();
	void update();
	bool notOutOfBound();
	Tri();
	~Tri();
};