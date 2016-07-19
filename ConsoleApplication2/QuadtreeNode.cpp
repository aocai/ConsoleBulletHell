#include "QuadtreeNode.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <mutex>

extern QuadtreeNode *qNode;
extern std::mutex objMtx;


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
	nw->minX = minX;
	nw->maxX = xCenter();
	nw->minY = minY;
	nw->maxY = yCenter();
	nw->nodeObjectVector = new std::vector<Object *>();

	ne = new QuadtreeNode();
	ne->parent = this;
	ne->minX = xCenter() + 1;
	ne->maxX = maxX;
	ne->minY = minY;
	ne->maxY = yCenter();
	ne->nodeObjectVector = new std::vector<Object *>();

	sw = new QuadtreeNode();
	sw->parent = this;
	sw->minX = minX;
	sw->maxX = xCenter();
	sw->minY = yCenter() + 1;
	sw->maxY = maxY;
	sw->nodeObjectVector = new std::vector<Object *>();

	se = new QuadtreeNode();
	se->parent = this;
	se->minX = xCenter() + 1;
	se->maxX = maxX;
	se->minY = yCenter() + 1;
	se->maxY = maxY;
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
	else if (onNodeCenters(obj))
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
	if ((obj->minX > maxX) || 
		(obj->maxX < minX) ||
		(obj->maxY < minY) ||
		(obj->minY > maxY)) 
	{
		return false;
	}
	return true;
}

bool QuadtreeNode::onNodeCenters(Object* obj)
{
	if ((obj->minX <= xCenter() && 
		obj->maxX > xCenter()) ||
		(obj->minY <= yCenter() && 
		obj->maxY > yCenter()))
	{
		return true;
	} 
	else 
	{
		return false;
	}
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
		if ((*nodeObjectVector)[i]->outOfBound())
		{

			//erase called here because of 50ms vs 100ms difference between object render and projectile render
			//this means objects are removed right away upon collision instead of waiting for object to update next frame
			(*nodeObjectVector)[i]->erase();

			std::swap((*nodeObjectVector)[i], nodeObjectVector->back());
			delete nodeObjectVector->back();
			nodeObjectVector->resize(nodeObjectVector->size() - 1);
			//nodeObjectVector->pop_back();
			--i;
		}
		else if (!objInQuadtreeNode((*nodeObjectVector)[i]))
		{
			//reassign the node through another tree traversal. Maybe optimize this with level by level?
			qNode->assignQNode((*nodeObjectVector)[i]);

			std::swap((*nodeObjectVector)[i], nodeObjectVector->back());
			//delete nodeObjectVector->back();
			nodeObjectVector->pop_back();
			--i;
		}
		else if (nw)
		{
			//check if on line
			if (!onNodeCenters((*nodeObjectVector)[i]))
			{
				//not on line. Call assignQNode() with this node as base
				assignQNode((*nodeObjectVector)[i]);

				std::swap((*nodeObjectVector)[i], nodeObjectVector->back());
				//delete nodeObjectVector->back();

				nodeObjectVector->pop_back();
				--i;
			}
			//else do nothing. node still in same nodeObjectVector (not a leaf node)
		}
		//if all else fail, this means the node is still in the same leaf node and nodeObjectVector
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
		(*nodeObjectVector)[i]->render();
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
