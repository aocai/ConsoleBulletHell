#include "ShortBeam.h"
#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Projectile.h"
#include "QuadtreeNode.h"

ShortBeam::ShortBeam()
{
}

ShortBeam::~ShortBeam()
{
}

extern HANDLE handle;
extern std::mutex consoleMtx;

void ShortBeam::spawnProj(int x, int y) 
{
	width = 1;
	height = 3;
	minX = x + 2; //center beam
	maxX = minX + width - 1;
	minY = y - height; //no -1 since y = maxY + 1
	maxY = minY + height - 1;
	speedX = 0;
	speedY = -3;
	first = true;
	collision = false;
}

//erase in console
void ShortBeam::erase() 
{

	COORD coord;
	coord.X = minX;
	coord.Y = maxY;

	//erase last render bottom up
	consoleMtx.lock();
	for (int i = 0; i < height; ++i) 
	{
		coord.Y = maxY - i;
		SetConsoleCursorPosition(handle, coord);
		std::cout << " ";
		if (coord.Y <= 0)
			break;
	}
	consoleMtx.unlock();
}

//render in console
void ShortBeam::render() 
{

	COORD coord;
	coord.X = minX;
	coord.Y = maxY;

	//render new position bottom up
	consoleMtx.lock();
	for (int i = 0; i < height; ++i) 
	{
		coord.Y = maxY - i;
		SetConsoleCursorPosition(handle, coord);
		std::cout << "|";
		if (coord.Y <= 0)
			break;
	}
	consoleMtx.unlock();
}