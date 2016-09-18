#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Circ.h"
#include "Source.h"

Circ::Circ()
{
}


Circ::~Circ()
{
}

void Circ::spawn() 
{
	width = 3;
	height = 2;
	minX = rand() % MAX_WIDTH;
	minY = 0;
	maxX = minX + width - 1;
	maxY = minY + height - 1;
	speedX = rand() % 5 - 2;
	speedY = 1;
	first = true;
	collision = false;
}

//erase object using console
void Circ::erase() 
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
		std::cout << "   ";
		coord.Y++;
		if (coord.Y < MAX_HEIGHT)
		{
			SetConsoleCursorPosition(handle, coord);
			std::cout << "   ";
		}
		consoleMtx.unlock();
	}
}

//render object using console
void Circ::render() 
{
	if (!outOfBound()) 
	{
		COORD coord;
		coord.X = minX;
		coord.Y = minY;

		//render from top down
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << "OOO";
		coord.Y++;
		if (coord.Y < MAX_HEIGHT)
		{
			SetConsoleCursorPosition(handle, coord);
			std::cout << "OOO";
		}
		consoleMtx.unlock();
	}
}