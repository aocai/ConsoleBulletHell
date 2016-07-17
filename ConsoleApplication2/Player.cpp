#include <iostream>
#include "Windows.h"
#include "Player.h"
#include "Projectile.h"
#include "ShortBeam.h"
#include "NormalAtk.h"
#include <vector>
#include <mutex>

extern HANDLE handle;
//extern std::vector<Object *> *objVect1;
//extern std::vector<Object *> *objVect2;
extern std::vector<Projectile *> *projVect1;
extern std::vector<Projectile *> *projVect3;
extern std::mutex consoleMtx;
extern std::mutex objMtx;
extern std::mutex projMtx;
extern QuadtreeNode *qNode;

Player::Player()
{
}

Player::~Player()
{
}

void Player::spawn() 
{
	posX = 38;
	posY = 70;
	width = 5;
	height = 1;
	speedX = 0;
	speedY = 0;
	markForErase = false;
	collision = false;
	first = false;
}

bool Player::updateAndRender() 
{
	erase();

	//get new position
	posX += speedX;
	posY += speedY;

	speedX = 0;
	speedY = 0;

	//out of bound test
	if (posY > 71) 
	{
		posY = 71;
	}
	else if (posY < 0) 
	{
		posY = 0;
	}
	if	(posX > 80 - width) 
	{
		posX = 80 - width;
	}
	else if (posX < 0) 
	{
		posX = 0;
	}

	//render new position
	render();

	return true;
}

void Player::update()
{
	//get new position
	posX += speedX;
	posY += speedY;
}

void Player::erase() 
{

	COORD coord;
	//erase last render
	coord.X = posX;
	coord.Y = posY;
	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "     ";
	consoleMtx.unlock();
}

void Player::render() 
{	//out of bound test
	if (posY > 71)
	{
		posY = 71;
	}
	else if (posY < 0)
	{
		posY = 0;
	}
	if (posX > 80 - width)
	{
		posX = 80 - width;
	}
	else if (posX < 0)
	{
		posX = 0;
	}
	COORD coord;
	//render new position
	coord.X = posX;
	coord.Y = posY;
	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "=====";
	consoleMtx.unlock();
}

bool Player::playerInQuadtreeNode(QuadtreeNode *qNode)
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


bool Player::collisionDetection(QuadtreeNode *node)
{
	if (!playerInQuadtreeNode(node))
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
		//if player and object do not touch...
		//TODO: change to check if prev.position - current.position emcompasses the object...
		if ((posX > obj->posX + obj->width - 1) ||
			(posX + width - 1 < obj->posX) ||
			(posY > obj->posY + obj->height - 1) ||
			(posY + height - 1 < obj->posY))
		{
			//no collision, keep player
			return false;
		}
		else
		{
			//player collision detected.
			return true;
		}
	}
	if (node->nw == NULL)
	{
		return false;
	}
	if (collisionDetection(node->nw) ||
		collisionDetection(node->ne) ||
		collisionDetection(node->sw) ||
		collisionDetection(node->se))
	{
		return true;
	}
	return false;
}

void Player::spawnShortBeam() 
{
	projMtx.lock();
	Projectile *shortBeam = new ShortBeam();
	shortBeam->spawnProj(posX, posY);

	//track if return true. Else free it
	if (shortBeam->updateAndRender()) 
	{
		projVect1->push_back(shortBeam);
	}
	else {
		delete shortBeam;
	}
	projMtx.unlock();
}

void Player::spawnNormalAtk() 
{
	Projectile *normalAtkLeft = new NormalAtk();
	Projectile *normalAtkRight = new NormalAtk();
	normalAtkLeft->spawnProj(posX, posY);
	normalAtkRight->spawnProj(posX + 4, posY);

	projMtx.lock();
	//track if return true. Else free it
	if (normalAtkLeft->updateAndRender()) 
	{
		projVect1->push_back(normalAtkLeft);
	}
	else {
		delete normalAtkLeft;
	}
	if (normalAtkRight->updateAndRender()) 
	{
		projVect1->push_back(normalAtkRight);
	}
	else {
		delete normalAtkRight;
	}
	projMtx.unlock();
}

void Player::ult() 
{
	objMtx.lock();

	qNode->updateObject();
	qNode->eraseAllObjects();

	//for (unsigned int i = 0; i < objVect1->size(); ++i) 
	//{
	//	(*objVect1)[i]->erase();

	//}

	//for (std::vector<Object *>::iterator it = objVect2->begin(); it != objVect2->end(); ++it) 
	//{
	//	delete *it;
	//}
	//object pointers in objVect1 but not in objVect2 are in objVect3, and will be freed later on
	//objVect1->clear();
	//objVect2->clear();

	objMtx.unlock();
}