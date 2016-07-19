#include "NormalAtk.h"
#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Projectile.h"
#include "QuadtreeNode.h"

NormalAtk::NormalAtk()
{
}

NormalAtk::~NormalAtk()
{
}

extern HANDLE handle;
extern std::mutex consoleMtx;

void NormalAtk::spawnProj(int x, int y) 
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
}

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
