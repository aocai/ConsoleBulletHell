#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Circ.h"

extern HANDLE handle;
extern std::mutex consoleMtx;

Circ::Circ()
{
}


Circ::~Circ()
{
}

void Circ::spawn() 
{
	width = 1;
	height = 1;
	minX = rand() % 80;
	minY = 0;
	maxX = minX + width - 1;
	maxY = minY + height - 1;
	speedX = rand() % 5 - 2;
	speedY = 1;
	first = true;
}

void Circ::erase() 
{
	if (!outOfBound()) {
		COORD coord;
		//erase last render
		coord.X = minX;
		coord.Y = minY;

		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << " ";
		consoleMtx.unlock();
	}
}

void Circ::render() 
{
	if (!outOfBound()) {
		COORD coord;
		coord.X = minX;
		coord.Y = minY;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << "O";
		consoleMtx.unlock();
	}
}