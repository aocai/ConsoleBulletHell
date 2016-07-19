#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Projectile.h"
#include "QuadtreeNode.h"

extern int finalScore;

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

bool Projectile::notOutOfBound()
{
	//out of bound test for whole beam
	//currently beams are erased right away if out of bound on X axis without non out of bound segment rendering
	if ((minY < 0) || (maxY > 71) || (minX < 0) || (maxX > 80))
	{
		return false;
	}
	return true;

}

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

bool Projectile::collisionTest(QuadtreeNode *node)
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
		Object *obj = (*node->nodeObjectVector)[i];
		//if projectile and object do not touch...
		//TODO: change to check if prev.position - current.position emcompasses the object...
		if ((minX > obj->maxX) ||
			(maxX < obj->minX) ||
			(maxY < obj->minY) ||
			(minY > obj->maxY))
		{
			//keep projectile
			return false;
		}
		else
		{
			//collision detected.
			//if ((*node->nodeObjectVector)[i]->collision == true)
			//{
			//	//only one projectile can be colliding 
			//	return false;
			//}
			//(*node->nodeObjectVector)[i]->collision = true;
			(*node->nodeObjectVector)[i]->erase();

			std::swap((*node->nodeObjectVector)[i], node->nodeObjectVector->back());
			delete node->nodeObjectVector->back();
			node->nodeObjectVector->resize(node->nodeObjectVector->size() - 1);
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