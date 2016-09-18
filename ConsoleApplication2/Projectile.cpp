#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Projectile.h"
#include "Object.h"
#include "QuadtreeNode.h"
#include "Source.h"

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::spawnProj(int x, int y) 
{
}

void Projectile::erase() 
{
}

void Projectile::update()
{
	minX += speedX;
	maxX += speedX;
	minY += speedY;
	maxY += speedY;
}

void Projectile::render() 
{
}

//return true if projectile is not out of bound
bool Projectile::notOutOfBound()
{
	//out of bound test
	if ((minY < 0) || (maxY > MAX_HEIGHT) || (minX < 0) || (maxX > MAX_WIDTH))
	{
		return false;
	}
	return true;

}

//return true if projectile touches node
bool Projectile::projInQuadtreeNode(QuadtreeNode *node)
{
	if ((minX > node->maxX) ||
		(maxX < node->minX) ||
		(maxY < node->minY) ||
		(minY > node->maxY))
	{
		//does not touch, return false
		return false;
	}
	return true;
}

//check if projectile collide with object
void Projectile::collisionTest(QuadtreeNode *node)
{
	//reject all child nodes if projectile is not in this node
	if (!projInQuadtreeNode(node))
	{
		return;
	}

	//if child nodes exist, recurse through
	if (node->nw)
	{
		collisionTest(node->nw);
		collisionTest(node->ne);
		collisionTest(node->sw);
		collisionTest(node->se);
	}

	for (unsigned int i = 0; i < node->nodeObjectVector->size(); ++i)
	{
		Object *obj = (*node->nodeObjectVector)[i];

		if (((minX >= obj->minX && minX <= obj->maxX) || (maxX >= obj->minX && maxX <= obj->maxX)) &&
			((minY >= obj->minY && minY <= obj->maxY) || (maxY >= obj->minY && maxY <= obj->maxY)))
		{
			//collision detected!
			if (!obj->collision)
			{
				//only flag if this is the first projectile to collide with object
				obj->collision = true;
				collision = true;
				finalScore++;
			}
		}
	}
}