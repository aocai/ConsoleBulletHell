#include "QuadtreeNode.h"
#include "Object.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <mutex>
#include "Source.h"

QuadtreeNode::QuadtreeNode()
{
}

QuadtreeNode::~QuadtreeNode()
{
}

//create sub nodes nw, ne, sw, se and object vector
void QuadtreeNode::createSubNodes() 
{
	nw = new QuadtreeNode();
	nw->parent = this;
	nw->minX = minX;
	nw->maxX = xCenter();
	nw->minY = minY;
	nw->maxY = yCenter();

	ne = new QuadtreeNode();
	ne->parent = this;
	ne->minX = xCenter() + 1;
	ne->maxX = maxX;
	ne->minY = minY;
	ne->maxY = yCenter();

	sw = new QuadtreeNode();
	sw->parent = this;
	sw->minX = minX;
	sw->maxX = xCenter();
	sw->minY = yCenter() + 1;
	sw->maxY = maxY;

	se = new QuadtreeNode();
	se->parent = this;
	se->minX = xCenter() + 1;
	se->maxX = maxX;
	se->minY = yCenter() + 1;
	se->maxY = maxY;
}

//assign the object to the correct quadtree node
void QuadtreeNode::assignQNode(Object* obj) 
{
	if (!objInQuadtreeNode(obj)) 
	{
		//trivially rejects all child nodes
		return;
	}
	else if (nw == NULL) 
	{
		//object lies in the leaf node
		nodeObjectVector.push_back(obj);
		return;
	}
	else if (onNodeCenters(obj))
	{
		//object lies on boundary between two nodes
		nodeObjectVector.push_back(obj);
		return;
	}
	else
	{
		//check child nodes if all ifs fail
		nw->assignQNode(obj);
		ne->assignQNode(obj);
		sw->assignQNode(obj);
		se->assignQNode(obj);
	}
}

//return false for objects on node boundaries
bool QuadtreeNode::objInQuadtreeNode(Object *obj) 
{
	if ((obj->minX >= minX) && 
		(obj->maxX <= maxX) && 
		(obj->minY >= minY) && 
		(obj->maxY <= maxY))
	{
		return true;
	}
	return false;
}

//check if object lies on the quad node center
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

//update objects in quadtree, and move them to new nodes if necessary
void QuadtreeNode::updateQuadtree()
{
	//if there are child nodes
	if (nw) 
	{
		nw->updateQuadtree();
		ne->updateQuadtree();
		sw->updateQuadtree();
		se->updateQuadtree();
	}

	for (unsigned int i = 0; i < nodeObjectVector.size(); ++i)
	{
		//object is out of bound or there has been a collision. object is removed from tree
		if (nodeObjectVector[i]->outOfBound() || nodeObjectVector[i]->collision)
		{
			std::swap(nodeObjectVector[i], nodeObjectVector.back());
			delete nodeObjectVector.back();
			nodeObjectVector.resize(nodeObjectVector.size() - 1);
			--i;
		}
		//check if object is still in this quadtree node
		else if (!objInQuadtreeNode(nodeObjectVector[i]))
		{
			//object is in another quadtree node. reassign the node through another tree traversal from base node, qNode
			qNode->assignQNode(nodeObjectVector[i]);

			std::swap(nodeObjectVector[i], nodeObjectVector.back());
			nodeObjectVector.pop_back();
			--i;
		}
		//check if its the leaf node
		else if (nw)
		{
			//object is in this node, and there are child nodes
			//check if on sub node boundaries
			if (!onNodeCenters(nodeObjectVector[i]))
			{
				//not on sub node boundaries. Call assignQNode() with this node as base
				assignQNode(nodeObjectVector[i]);

				std::swap(nodeObjectVector[i], nodeObjectVector.back());
				nodeObjectVector.pop_back();
				--i;
			}
			//else do nothing. node still in same nodeObjectVector (not a leaf node)
		}
		//if all else fail, this means the node is still in the same leaf node and nodeObjectVector
	}
}

//render all objects in quadtree through recursion
void QuadtreeNode::renderFromTree()
{
	if (nw)
	{
		nw->renderFromTree();
		ne->renderFromTree();
		sw->renderFromTree();
		se->renderFromTree();
	}
	for (unsigned int i = 0; i < nodeObjectVector.size(); ++i)
	{
		nodeObjectVector[i]->render();
	}
}

//erase object's current render on console, and update position based on speed in X and Y
void QuadtreeNode::updateObject()
{
	if (nw) 
	{
		nw->updateObject();
		ne->updateObject();
		sw->updateObject();
		se->updateObject();
	}
	for (unsigned int i = 0; i < nodeObjectVector.size(); ++i)
	{
		Object *obj = nodeObjectVector[i];
		if (!obj->first)
		{
			//obj->erase();
			if (!obj->collision)
			{
				obj->update();
			}
		}
		else
		{
			//object just initialized, do not need to erase and update position
			obj->first = false;
		}
	}
}

//erase all objects in quadtree
void QuadtreeNode::eraseAllObjects()
{
	if (nw)
	{
		nw->eraseAllObjects();
		ne->eraseAllObjects();
		sw->eraseAllObjects();
		se->eraseAllObjects();
	}
	for (unsigned int i = 0; i < nodeObjectVector.size(); ++i)
	{
		delete nodeObjectVector[i];
	}

	nodeObjectVector.clear();
}

//free quadtree memory. called at game's end
void QuadtreeNode::freeQuadtree()
{
	if (nw)
	{
		nw->freeQuadtree();
		ne->freeQuadtree();
		sw->freeQuadtree();
		se->freeQuadtree();
	}
	if (nw)
	{
		delete nw;
		delete ne;
		delete sw;
		delete se;
	}
}