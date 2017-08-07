#pragma once

class Object
{
public:
	int minX, maxX;
	int minY, maxY;
	int height, width;
	int speedX; //right = positive
	int speedY; //down = positive
	bool first; //just initialized?
	bool collision;
	virtual void spawn();
	virtual void erase();
	virtual void render();
	virtual void update();
	virtual bool outOfBound();
	Object();
	virtual ~Object();
};
