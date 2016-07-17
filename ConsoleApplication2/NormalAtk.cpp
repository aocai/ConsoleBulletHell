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
extern QuadtreeNode *qNode;
extern int finalScore;

void NormalAtk::spawnProj(int x, int y) 
{
	posX = x;
	posY = y - 1; //move 1 up from player
	width = 1;
	height = 1;
	speedX = 0;
	speedY = -1;
	first = 1;
}

bool NormalAtk::updateAndRender() 
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

	//out of bound test
	if (posY < 0) 
	{
		return false;
	}

	//TODO: UNCOMMENT WHEN DONE WITH NODE ALLOCATION
	//if (collisionTest(qNode)) 
	//{
	//	return false;
	//}

	//render new position
	render();

	return true;
}

void NormalAtk::erase() 
{

	COORD coord;
	coord.X = posX;
	coord.Y = posY;

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
	coord.X = posX;
	coord.Y = posY;

	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "^";
	consoleMtx.unlock();
}

bool NormalAtk::projInQuadtreeNode(QuadtreeNode *qNode) 
{
	if ((posX > qNode->xMax) ||
		(posX + width  - 1 < qNode->xMin) ||
		(posY < qNode->yMin) ||
		(posY + height - 1 > qNode->yMax)) 
	{
		//does not touch
		return false;
	}
	return true;
}


bool NormalAtk::collisionTest(QuadtreeNode *node)
{
	if (!projInQuadtreeNode(node))
	{
		return false;
	}
	if (!node->nodeObjectVector)
	{
		return false;
	}
	for (unsigned int i = 0; i < node->nodeObjectVector->size(); ++i)
	{
		//Object *obj = qNode->nodeObjectVector->operator[](i);
		//WORKS LIKE THIS RIGHT??
		Object *obj = (*node->nodeObjectVector)[i];
		//if projectile and object do not touch...
		if ((posX > obj->posX + obj->width - 1) ||
			(posX + width - 1 < obj->posX) ||
			(posY > obj->posY) ||
			(posY + height - 1 > obj->posY + obj->height - 1)) 
		{
			//keep projectile, do nothing
			//return false;
		}
		else 
		{
			//collision detected.
			//remove the appropriate object in nodeObjectVector
			if ((*node->nodeObjectVector)[i]->collision == true)
			{
				//only one projectile can be colliding 
				return false;
			}
			finalScore++;
			(*node->nodeObjectVector)[i]->collision = true;
			return true;
		}
	}

	if (node->nw == NULL)
	{
		return false;
	}

	if (collisionTest(node->nw) ||
		collisionTest(node->ne) ||
		collisionTest(node->sw) ||
		collisionTest(node->se))
	{
		return true;
	}

	return false;
}

void NormalAtk::update()
{
	posX += speedX;
	posY += speedY;
}

bool NormalAtk::notOutOfBound()
{
	if (posY < 0)
	{
		return false;
	}
	return true;

}