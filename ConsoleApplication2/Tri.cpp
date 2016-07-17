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
	posX = rand() % 74;
	posY = 0;
	width = 6;
	height = 2;
	speedX = rand() % 5 - 2;
	speedY = 1;
	first = true;
	collision = false;
	markForErase = false;
}

bool Tri::updateAndRender() 
{

	if (!first)
	{
		//erase last render
		erase();

		//get new position
		posX += speedX;
		posY += speedY;
	}
	else
	{
		first = 0;
	}

	//out of bound tests
	if (posY + 1 > 71) 
	{
		return false;
	}
	if (posX + width > 80) 
	{
		speedX = -speedX;
		posX = 80 - width - (posX + width - 80);
	}
	else if (posX <= 0) 
	{
		speedX = -speedX;
		posX = -posX;
	}

	//render new position
	render();

	return true;
}

void Tri::erase() 
{

	COORD coord;
	//erase last render
	coord.X = posX;
	coord.Y = posY;

	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "    ";
	coord.Y = posY + height - 1;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "      ";
	consoleMtx.unlock();
}

void Tri::render() 
{

	COORD coord;
	coord.X = posX;
	coord.Y = posY;

	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "  /\\";
	coord.Y = posY + 1;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "/____\\";
	consoleMtx.unlock();
}

void Tri::update()
{

	//get new position
	posX += speedX;
	posY += speedY;

	if (posX + width > 80)
	{
		speedX = -speedX;
		//posX = 80 - (posX + width - 1 - 80);
		posX = 80 - width - (posX + width - 80);
	}
	else if (posX <= 0)
	{
		speedX = -speedX;
		posX = 0 - posX;
	}
}

bool Tri::notOutOfBound()
{
	//out of bound tests
	if (posY + 1 > 71)
	{
		return false;
	}
	return true;
}