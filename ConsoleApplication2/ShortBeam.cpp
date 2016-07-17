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
extern QuadtreeNode *qNode;
extern std::vector<Projectile *> *projVect1;
extern std::vector<Projectile *> *projVect2;
extern std::vector<Projectile *> *projVect3;
extern int finalScore;


void ShortBeam::spawnProj(int x, int y) 
{
	posX = x + 2; //center beam
	posY = y - 1; //move 1 up  from player
	width = 1;
	height = 3;
	speedX = 0;
	speedY = -3;
	first = 1;
}

bool ShortBeam::updateAndRender() 
{
	//erase last render
	if (!first) 
	{
		erase();
		//get new position
		posX += speedX;
		posY += speedY;
	}
	else 
	{
		first = 0;
	}

	//out of bound test for whole beam
	if (posY < 0) 
	{
		return false;
	}

//	if (collisionTest(qNode)) {
//		//collision detected, do not continue to render
//		return false;
//	}

	//render new position bottom first
	render();

	return true;
}

void ShortBeam::erase() 
{

	COORD coord;
	coord.X = posX;
	coord.Y = posY;

	//erase last render
	consoleMtx.lock();
	for (int i = 0; i < height; ++i) 
	{
		coord.Y = posY - i;
		SetConsoleCursorPosition(handle, coord);
		std::cout << " ";
		if (coord.Y <= 0)
			break;
	}
	consoleMtx.unlock();
}

void ShortBeam::render() 
{

	COORD coord;
	//render new position
	coord.X = posX;
	coord.Y = posY;
	consoleMtx.lock();
	coord.X = posX;
	coord.Y = posY;
	for (int i = 0; i < height; ++i) 
	{
		coord.Y = posY - i;
		SetConsoleCursorPosition(handle, coord);
		std::cout << "|";
		if (coord.Y <= 0) { break;}
	}
	consoleMtx.unlock();
}

bool ShortBeam::projInQuadtreeNode(QuadtreeNode *qNode) 
{
	if ((posX > qNode->xMax) ||
		(posX + width - 1 < qNode->xMin) ||
		(posY < qNode->yMin) ||
		(posY + height - 1 > qNode->yMax)) 
	{
		return false;
	}
	return true;
}

bool ShortBeam::collisionTest(QuadtreeNode *node) 
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
		//TODO: change to check if prev.position - current.position emcompasses the object...
		if ((posX > obj->posX + obj->width - 1) ||
			(posX + width - 1 < obj->posX) ||
			(posY > obj->posY) ||
			(posY + height - 1 > obj->posY + obj->height - 1)) 
		{
			//keep projectile
			return false;
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
			(*node->nodeObjectVector)[i]->collision = true;
			finalScore++;
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

void ShortBeam::update()
{
	posX += speedX;
	posY += speedY;
}

bool ShortBeam::notOutOfBound()
{
	if (posY < 0)
	{
		return false;
	}
	return true;
	
}