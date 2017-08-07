#pragma once
#include <vector>

class Object;

class QuadtreeNode
{
public:
	QuadtreeNode *parent, *nw, *ne, *sw, *se;
	int minX, maxX;
	int minY, maxY;
	int xCenter() const { return (minX + maxX) / 2; } //round down
	int yCenter() const { return (minY + maxY) / 2; }

	std::vector<Object *> nodeObjectVector;

	void createSubNodes();
	void assignQNode(Object* obj);
	bool objInQuadtreeNode(Object * obj);
	bool onNodeCenters(Object* obj);
	void updateQuadtree();
	void renderFromTree();
	void updateObject();
	void eraseAllObjects();
	void freeQuadtree();

	QuadtreeNode();
	~QuadtreeNode();
};

