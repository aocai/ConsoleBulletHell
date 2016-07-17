#include <iostream>
#include <mutex>
#include "Windows.h"
#include "Projectile.h"
#include "QuadtreeNode.h"

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::spawnProj(int x, int y) 
{
}

bool Projectile::updateAndRender() 
{
	return true;
}

void Projectile::erase() 
{
}

void Projectile::update()
{
}

void Projectile::render() 
{
}

bool Projectile::notOutOfBound()
{
	return true;
}

bool Projectile::projInQuadtreeNode(QuadtreeNode *qNode) 
{
	return false;
}

bool Projectile::collisionTest(QuadtreeNode *qNode) 
{
	return false;
}