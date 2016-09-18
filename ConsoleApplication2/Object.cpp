#include "Object.h"
#include "Source.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::spawn() 
{
}

void Object::erase() 
{
}

void Object::render() 
{
}

void Object::update()
{
	//get new position
	minX += speedX;
	maxX += speedX;
	minY += speedY;
	maxY += speedY;

	//change direction of object if out of bound
	if (maxX >= MAX_WIDTH)
	{
		speedX = -speedX;
		maxX = MAX_WIDTH - 1;
		minX = maxX - width + 1;
	}
	else if (minX < 0)
	{
		speedX = -speedX;
		minX = -minX;
		maxX = minX + width - 1;
	}
}

bool Object::outOfBound()
{
	//out of screen (Y direction) test
	if (minY < MAX_HEIGHT)
	{
		return false;
	}
	return true;
}