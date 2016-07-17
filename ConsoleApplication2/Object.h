#pragma once

class Object
{
public:
	int posX; //top left
	int posY; //top left
	int width;
	int height;
	int speedX; //right = positive
	int speedY; //down = positive
	bool first; //just initialized?
	bool collision; //collision detected?
	bool markForErase; //delete from tree if true
	virtual void spawn();
	virtual bool updateAndRender();
	virtual void erase();
	virtual void render();
	virtual void update();
	virtual bool notOutOfBound();
	Object();
	~Object();
};
