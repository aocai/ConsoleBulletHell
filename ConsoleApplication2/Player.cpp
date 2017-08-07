#include <iostream>
#include "Windows.h"
#include "Player.h"
#include "Projectile.h"
#include "ShortBeam.h"
#include "NormalAtk.h"
#include "QuadtreeNode.h"
#include "Source.h"
#include "Player.h"
#include <vector>
#include <mutex>

Player& Player::getInstance()
{
	static Player instance;
	return instance;
}

Player::~Player()
{
}

void Player::spawn() 
{
	width = 5;
	height = 1;
	minX = 38;
	minY = MAX_HEIGHT - 1;
	maxX = minX + width - 1;
	maxY = minY + height - 1;
	speedX = 0;
	speedY = 0;
	collision = false;
}

void Player::update()
{
	//bound player speed
	if (speedX > 3)
	{
		speedX = 3;
	}
	else if (speedX < -3)
	{
		speedX = -3;
	}
	if (speedY > 2)
	{
		speedY = 2;
	}
	else if (speedY < -2)
	{
		speedY = -2;
	}

	//get new position
	minX += speedX;
	maxX += speedX;
	minY += speedY;
	maxY += speedY;

	//out of bound test
	if (maxY >= MAX_HEIGHT)
	{
		maxY = MAX_HEIGHT - 1;
		minY = maxY - height + 1;
		speedY = 0;
	}
	else if (minY < 0)
	{
		minY = 0;
		maxY = minY + height - 1;
		speedY = 0;
	}
	if (maxX >= MAX_WIDTH)
	{
		maxX = MAX_WIDTH - 1;
		minX = maxX - width + 1;
		speedX = 0;
	}
	else if (minX < 0)
	{
		minX = 0;
		maxX = minX + width - 1;
		speedX = 0;
	}
}

//erase using console
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

//render using console
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

//check if player is in Quadtree node. Return true if any part of player touches node
bool Player::playerInQuadtreeNode(QuadtreeNode *node)
{
	if ((minX > node->maxX) ||
		(maxX < node->minX) ||
		(maxY < node->minY) ||
		(minY > node->maxY))
	{
		return false;
	}
	return true;
}

//check if player collided with an object, and if so, flag it
void Player::collisionDetection(QuadtreeNode *node)
{
	//reject all child nodes if player is not in this node
	if (!playerInQuadtreeNode(node))
	{
		return;
	}

	//if there are child nodes, recurse
	if (node->nw)
	{
		collisionDetection(node->nw);
		collisionDetection(node->ne);
		collisionDetection(node->sw);
		collisionDetection(node->se);
	}

	for (unsigned int i = 0; i < node->nodeObjectVector.size(); ++i)
	{
		Object *obj = (node->nodeObjectVector)[i];

		//check if object touches player
		if (((minX >= obj->minX && minX <= obj->maxX) || (maxX >= obj->minX && maxX <= obj->maxX)) &&
			((minY >= obj->minY && minY <= obj->maxY) || (maxY >= obj->minY && maxY <= obj->maxY)))
		{
			//collision detected
			collision = true;
		}
	}
}

//spawn new projectile and add to list of projectiles
void Player::spawnShortBeam() 
{
	Projectile *shortBeam = new ShortBeam();
	shortBeam->spawnProj(minX, minY);

	projMtx.lock();

	shortBeam->render();
	renderProjVector.push_back(shortBeam);

	projMtx.unlock();
}

//spawn new projectile and add to list of projectiles
void Player::spawnNormalAtk() 
{
	Projectile *normalAtkLeft = new NormalAtk();
	Projectile *normalAtkRight = new NormalAtk();
	normalAtkLeft->spawnProj(minX, minY);
	normalAtkRight->spawnProj(minX + 4, minY);

	projMtx.lock();

	normalAtkLeft->render();
	normalAtkRight->render();
	renderProjVector.push_back(normalAtkLeft);
	renderProjVector.push_back(normalAtkRight);

	projMtx.unlock();
}

//delete all projectiles
void Player::ult() 
{
	qNode->updateObject();
	qNode->eraseAllObjects();
}