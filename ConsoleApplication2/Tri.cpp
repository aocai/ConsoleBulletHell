#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Tri.h"

extern HANDLE handle;
extern std::mutex consoleMtx;

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
	minX = rand() % 74;
	minY = 0;
	maxX = minX + width - 1;
	maxY = minY + height - 1;
	speedX = rand() % 5 - 2;
	speedY = 1;
	first = true;
}

void Tri::erase() 
{
	if (!outOfBound()) {
		COORD coord;
		//erase last render
		coord.X = minX;
		coord.Y = minY;

		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << "    ";

		coord.Y += 1;
		if (coord.Y <= 71) {
			SetConsoleCursorPosition(handle, coord);
			std::cout << "      ";
		}
		consoleMtx.unlock();
	}
}

void Tri::render() 
{
	if (!outOfBound()) {
		COORD coord;
		coord.X = minX;
		coord.Y = minY;

		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << "  /\\";

		coord.Y += 1;
		if (coord.Y <= 71) {
			SetConsoleCursorPosition(handle, coord);
			std::cout << "/____\\";
		}
		consoleMtx.unlock();
	}
}