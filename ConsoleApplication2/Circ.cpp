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
	posX = rand() % 80;
	posY = 0;
	width = 1;
	height = 1;
	speedX = rand() % 5 - 2;
	speedY = 1;
	first = true;
	collision = false;
	markForErase = false;
}

bool Circ::updateAndRender() 
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
	if (posY > 71) 
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

void Circ::erase() 
{

	COORD coord;
	//erase last render
	coord.X = posX;
	coord.Y = posY;

	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << " ";
	consoleMtx.unlock();
}

void Circ::render() 
{

	COORD coord;
	coord.X = posX;
	coord.Y = posY;
	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "O";
	consoleMtx.unlock();
}

void Circ::update()
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

bool Circ::notOutOfBound()
{
	//out of bound tests
	if (posY + height - 1 > 71)
	{
		return false;
	}
	return true;
}