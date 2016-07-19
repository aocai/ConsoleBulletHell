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
	width = 5;
	height = 1;
	minX = 38;
	minY = 70;
	maxX = minX + width - 1;
	maxY = minY + height - 1;
	speedX = 0;
	speedY = 0;
}

void Player::update()
{
	//get new position
	minX += speedX;
	maxX += speedX;
	minY += speedY;
	maxY += speedY;

	speedX = 0;
	speedY = 0;

	//out of bound test
	if (maxY > 71)
	{
		maxY = 71;
		minY = maxY - height + 1;
	}
	else if (minY < 0)
	{
		minY = 0;
		maxY = minY + height - 1;
	}
	if (maxX > 79)
	{
		maxX = 79;
		minX = maxX - width + 1;
	}
	else if (minX < 0)
	{
		minX = 0;
		maxX = minX + width - 1;
	}
}

void Player::erase() 
{

	COORD coord;
	//erase last render
	coord.X = minX;
	coord.Y = minY;
	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "     ";
	consoleMtx.unlock();
}

void Player::render() 
{	
	COORD coord;
	//render new position
	coord.X = minX;
	coord.Y = minY;
	consoleMtx.lock();
	SetConsoleCursorPosition(handle, coord);
	std::cout << "=====";
	consoleMtx.unlock();
}

bool Player::playerInQuadtreeNode(QuadtreeNode *qNode)
{
	if ((minX > qNode->maxX) ||
		(maxX < qNode->minX) ||
		(maxY < qNode->minY) ||
		(minY > qNode->maxY))
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
		Object *obj = (*node->nodeObjectVector)[i];
		//if player and object do not touch...
		//TODO: change to check if prev.position - current.position emcompasses the object...
		if ((minX > obj->maxX) ||
			(maxX < obj->minX) ||
			(minY > obj->maxY) ||
			(maxY < obj->minY))
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
	Projectile *shortBeam = new ShortBeam();
	shortBeam->spawnProj(minX, minY);

	projMtx.lock();

	shortBeam->render();
	projVect1->push_back(shortBeam);

	projMtx.unlock();
}

void Player::spawnNormalAtk() 
{
	Projectile *normalAtkLeft = new NormalAtk();
	Projectile *normalAtkRight = new NormalAtk();
	normalAtkLeft->spawnProj(minX, minY);
	normalAtkRight->spawnProj(minX + 4, minY);

	projMtx.lock();

	normalAtkLeft->render();
	normalAtkRight->render();
	projVect1->push_back(normalAtkLeft);
	projVect1->push_back(normalAtkRight);

	projMtx.unlock();
}

void Player::ult() 
{

	qNode->updateObject();
	qNode->eraseAllObjects();

}