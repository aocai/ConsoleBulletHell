#pragma once
#include "Object.h"
#include <vector>

class QuadtreeNode
{
public:
	QuadtreeNode *parent, *nw, *ne, *sw, *se;
	int xMin, xMax;
	int yMin, yMax;
	int xCenter() const { return (xMin + xMax) / 2; } //round down
	int yCenter() const { return (yMin + yMax) / 2; }

	//Projectile *firstProjectile;
	Object *firstObject;
	std::vector<Object *> *nodeObjectVector;

	void createSubNodes();
	void assignQNode(Object* obj);
	bool objInQuadtreeNode(Object * obj);
	void updateQuadtree();
	void renderFromTree();
	void updateObject();
	void eraseAllObjects();

	QuadtreeNode();
	~QuadtreeNode();
};

