#include "Object.h"
#include <iostream>

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

	if (maxX > 79)
	{
		speedX = -speedX;
		maxX = 79 - (maxX - 79);
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
	//out of bound test
	if (minY <= 71)
	{
		return false;
	}
	return true;
}