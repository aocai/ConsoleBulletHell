#include "QuadtreeNode.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>

extern QuadtreeNode *qNode;


QuadtreeNode::QuadtreeNode()
{
}

QuadtreeNode::~QuadtreeNode()
{
}


void QuadtreeNode::createSubNodes() 
{

	nw = new QuadtreeNode();
	nw->parent = this;
	nw->xMin = xMin;
	nw->xMax = xCenter();
	nw->yMin = yMin;
	nw->yMax = yCenter();
	nw->nodeObjectVector = new std::vector<Object *>();

	ne = new QuadtreeNode();
	ne->parent = this;
	ne->xMin = xCenter() + 1;
	ne->xMax = xMax;
	ne->yMin = yMin;
	ne->yMax = yCenter();
	ne->nodeObjectVector = new std::vector<Object *>();

	sw = new QuadtreeNode();
	sw->parent = this;
	sw->xMin = xMin;
	sw->xMax = xCenter();
	sw->yMin = yCenter() + 1;
	sw->yMax = yMax;
	sw->nodeObjectVector = new std::vector<Object *>();

	se = new QuadtreeNode();
	se->parent = this;
	se->xMin = xCenter() + 1;
	se->xMax = xMax;
	se->yMin = yCenter() + 1;
	se->yMax = yMax;
	se->nodeObjectVector = new std::vector<Object *>();
}

void QuadtreeNode::assignQNode(Object* obj) 
{
	if (!objInQuadtreeNode(obj)) 
	{
		//trivially rejects all child nodes
		return;
	}

	if (nw == NULL) 
	{
		//object lies in the leaf node
		nodeObjectVector->push_back(obj);
		return;
	}
	else if (((obj->posX <= xCenter()) && (obj->posX + obj->width - 1 > xCenter())) ||
		((obj->posY <= yCenter()) && (obj->posY + obj->height - 1 > yCenter()))) 
	{
		//object lies on boundary between two nodes
		nodeObjectVector->push_back(obj);
		return;
	}
	//check child nodes if all ifs fail
	nw->assignQNode(obj);
	ne->assignQNode(obj);
	sw->assignQNode(obj);
	se->assignQNode(obj);
}

bool QuadtreeNode::objInQuadtreeNode(Object *obj) 
{
	if ((obj->posX > xMax) || 
		(obj->posX + obj->width - 1 < xMin) ||
		(obj->posY + obj->height - 1 < yMin) ||
		(obj->posY > yMax)) 
	{
		return false;
	}
	return true;
}

void QuadtreeNode::updateQuadtree()
{
	if (nw) 
	{
		nw->updateQuadtree();
		ne->updateQuadtree();
		sw->updateQuadtree();
		se->updateQuadtree();
	}

	for (unsigned int i = 0; i < nodeObjectVector->size(); ++i)
	{
		Object *obj = (*nodeObjectVector)[i];
		if (obj->markForErase || !obj->notOutOfBound())
		{
			delete obj;
			nodeObjectVector->erase(nodeObjectVector->begin() + i);
		}
		else if (!objInQuadtreeNode(obj))
		{
			//reassign the node through another tree traversal. Maybe optimize this with level by level?
			qNode->assignQNode(obj);
			nodeObjectVector->erase(nodeObjectVector->begin() + i);
			//std::swap(obj, nodeObjectVector->back());
			//delete nodeObjectVector->back();
			//nodeObjectVector->pop_back();
		}
		else if (nw)
		{
			//check if on line, else call assignQNode() with this node as base
			if (!(((obj->posX <= xCenter()) && (obj->posX + obj->width - 1 > xCenter())) ||
				((obj->posY <= yCenter()) && (obj->posY + obj->height - 1 > yCenter()))))
			{
				//not on line
				assignQNode(obj);
				nodeObjectVector->erase(nodeObjectVector->begin() + i);
				//std::swap(obj, nodeObjectVector->back());
				//delete nodeObjectVector->back();
				//nodeObjectVector->pop_back();
			}
		}
	}
}

void QuadtreeNode::renderFromTree()
{
	if (nw)
	{
		nw->renderFromTree();
		ne->renderFromTree();
		sw->renderFromTree();
		se->renderFromTree();
	}
	for (unsigned int i = 0; i < nodeObjectVector->size(); ++i)
	{
		if (!((*nodeObjectVector)[i]->markForErase || (*nodeObjectVector)[i]->collision))
		{
			(*nodeObjectVector)[i]->render();
		}
	}
}

void QuadtreeNode::updateObject()
{
	if (nw) 
	{
		nw->updateObject();
		ne->updateObject();
		sw->updateObject();
		se->updateObject();
	}
	for (unsigned int i = 0; i < nodeObjectVector->size(); ++i)
	{
		Object *obj = (*nodeObjectVector)[i];
		if (!obj->first)
		{
			obj->erase();
			obj->update();
		}
		else
		{
			obj->first = false;
		}
		if (obj->collision)
		{
			obj->markForErase = true;
		}
	}
}

void QuadtreeNode::eraseAllObjects()
{
	if (nw)
	{
		nw->eraseAllObjects();
		ne->eraseAllObjects();
		sw->eraseAllObjects();
		se->eraseAllObjects();
	}
	for (unsigned int i = 0; i < nodeObjectVector->size(); ++i)
	{
		delete (*nodeObjectVector)[i];
	}
	nodeObjectVector->clear();
}


//updateQuadtreeNode() has kinda same performance as building a new tree from scratch...
//void QuadtreeNode::updateQuadtreeNode() 
//{
//	if (nw == NULL) 
//	{
//		for (int i = 0; i < nodeObjectVector->size(); ++i)
//		{
//			if (!objInQuadtreeNode((*nodeObjectVector)[i]))
//			{
//
//			}
//		}
//	}
//}

//instead of "deleting" Object right away when they go out of screen (by adding and removing from objVect), 
//"delete" them later on before rendering by traversing Quadtree and render all in every nodeVector. This way
//no daggling pointers

//void QuadtreeNode::raytrace() {
//	for (int i = 0; i < projVect1->size(); ++i) {
//		//if do hit object in qNode
//		if (qNode->collisionTest((*projVect1)[i])) {
//			//remove projectile
//			projVect3->push_back((*projVect1)[i]);
//		}
//		else {
//			projVect2->push_back((*projVect1)[i]);
//		}
//	}
//}