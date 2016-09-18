#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Tri.h"
#include "Source.h"

Tri::Tri()
{
}

Tri::~Tri()
{
}

void Tri::spawn() 
{
	width = 6;
	height = 2;
	minX = rand() % (MAX_WIDTH - width);
	minY = 0;
	maxX = minX + width - 1;
	maxY = minY + height - 1;
	speedX = rand() % 5 - 2;
	speedY = 1;
	first = true;
	collision = false;
}

//erase object using console
void Tri::erase() 
{
	if (!outOfBound()) 
	{
		COORD coord;
		//erase last render
		coord.X = minX;
		coord.Y = minY;

		//render from top down
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << "    ";

		coord.Y++;
		if (coord.Y < MAX_HEIGHT) {
			SetConsoleCursorPosition(handle, coord);
			std::cout << "      ";
		}
		consoleMtx.unlock();
	}
}

//render object using console
void Tri::render() 
{
	if (!outOfBound()) 
	{
		COORD coord;
		coord.X = minX;
		coord.Y = minY;

		//render from top down
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << "  /\\";

		coord.Y++;
		if (coord.Y < MAX_HEIGHT)
		{
			SetConsoleCursorPosition(handle, coord);
			std::cout << "/____\\";
		}
		consoleMtx.unlock();
	}
}