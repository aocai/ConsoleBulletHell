#include "NormalAtk.h"
#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Projectile.h"
#include "QuadtreeNode.h"
#include "Source.h"

NormalAtk::NormalAtk(int x, int y)
{
	width = 1;
	height = 1;
	minX = x;
	maxX = minX + width - 1;
	minY = y - height; //no -1 since y = maxY + 1
	maxY = minY + height - 1;
	speedX = 0;
	speedY = -1;
	first = true;
	collision = false;
}

NormalAtk::~NormalAtk()
{
}

//erase in console
void NormalAtk::erase() 
{

	COORD coord;
	coord.X = minX;
	coord.Y = maxY;

	//erase last render
	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << " ";
	consoleMtx.unlock();
}

//render in console
void NormalAtk::render() 
{

	COORD coord;
	//render new position
	coord.X = minX;
	coord.Y = maxY;

	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "^";
	consoleMtx.unlock();
}
