#include <stdlib.h>
#include <stdio.h>
#include "windows.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <thread>
#include "Source.h"
#include <algorithm> //for testing

#define _WIN32_WINNT 0x500

bool isGameOver = false;

void monitorKB(Player *player)
{
	int KB_code = 0;
	while (KB_code != 27) 
	{
		if (_kbhit()) 
		{
			KB_code = _getch();
			if (!isGameOver)
			{
				switch (KB_code)
				{
					//KB_LEFT
				case 75:
					player->speedX = -2;
					player->speedY = 0;
					//player->updateAndRender();
					player->erase();
					player->update();
					player->render();
					if (player->collisionDetection(qNode))
					{
						isGameOver = true;
						gameOver();
					}
					break;
					//KB_RIGHT
				case 77:
					player->speedX = 2;
					player->speedY = 0;
					//player->updateAndRender();
					player->erase();
					player->update();
					player->render();
					if (player->collisionDetection(qNode))
					{
						isGameOver = true;
						gameOver();
					}
					break;
					//KB_UP
				case 72:
					player->speedX = 0;
					player->speedY = -1;
					//player->updateAndRender();
					player->erase();
					player->update();
					player->render();
					if (player->collisionDetection(qNode))
					{
						isGameOver = true;
						gameOver();
					}
					break;
					//KB_DOWN
				case 80:
					player->speedX = 0;
					player->speedY = 1;
					//player->updateAndRender();
					player->erase();
					player->update();
					player->render();
					if (player->collisionDetection(qNode))
					{
						isGameOver = true;
						gameOver();
					}
					break;
					//KB_Z
				case 122:
				case 90:
					player->spawnNormalAtk();
					break;
					//KB_X
				case 120:
				case 88:
					player->spawnShortBeam();
					break;
					//KB_SPACE
				case 32:
					player->ult();
					break;
				}
			}
			else
			{
				switch (KB_code)
				{
					//KB_ENTER
				case 13:
					//restart
					break;
				}
			}
		}
	}
}

void monitorProj() 
{
	while (!isGameOver)
	{

		//for (unsigned int i = 0; i < projVect1->size(); ++i) 
		//{
		//	Projectile *proj = (*projVect1)[i];
		//	if (proj->updateAndRender()) 
		//	{
		//		projVect2->push_back(proj);
		//	}
		//	else 
		//	{
		//		projVect3->push_back(proj);
		//	}
		//}

		projMtx.lock();

		for (unsigned int i = 0; i < projVect1->size(); ++i)
		{

			Projectile *proj = (*projVect1)[i];
			if (!proj->first)
			{
				//erase the render and update the position
				proj->erase();
				proj->update();
			}
			else
			{
				proj->first = false;
			}
			if (proj->notOutOfBound()) {
				//proj to be tested for collision
				projVect2->push_back(proj);
			}
			else
			{
				//proj to be thrown away
				projVect3->push_back(proj);
			}
		}

		projVect1->clear();

		//update tree
		qNode->updateQuadtree();

		//check collision. Both objects and projectiles are rejected here.
		for (unsigned int i = 0; i < projVect2->size(); ++i)
		{
			Projectile *proj = (*projVect2)[i];
			if (proj->collisionTest(qNode))
			{
				//collision detected, throw away projectile
				projVect3->push_back(proj);
			}
			else
			{
				projVect1->push_back(proj);
			}
		}

		for (unsigned int i = 0; i < projVect1->size(); ++i)
		{
			(*projVect1)[i]->render();
		}

		projVect2->clear();

		for (std::vector<Projectile *>::iterator it = projVect3->begin(); it != projVect3->end(); ++it) 
		{
			delete *it;
		}
		projVect3->clear();

		projMtx.unlock();
		
		Sleep(50);
	}
}


void gameOver()
{
	player->erase();

	qNode->updateObject();
	qNode->eraseAllObjects();

	for (unsigned int i = 0; i < projVect1->size(); ++i)
	{
		(*projVect1)[i]->erase();
	}

	COORD coord;
	//render new position
	coord.X = 35;
	coord.Y = 30;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "GAME OVER!";
	coord.Y += 1;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "SCORE: " << finalScore;
	coord.Y += 1;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "PRESS ENTER TO RESTART!";
}

void setup() 
{
	while (!isGameOver)
	{
		objMtx.lock();
		//random generate objects
		if (rand() % 5 == 0) 
		{
			Object *tri = new Tri();
			tri->spawn();
			qNode->assignQNode(tri);
			//Do i still do this??...
			//objVect1->push_back(tri);
		}
		if (rand() % 5 == 0) 
		{
			Object *circ = new Circ();
			circ->spawn();
			qNode->assignQNode(circ);
			//objVect1->push_back(circ);
		}

		//call update and keep track of useful objects and objects to be freed

		//implementation #1
		//for (unsigned int i = 0; i < objVect1->size(); ++i) 
		//{
		//	Object *obj = (*objVect1)[i];
		//	if (obj && obj->updateAndRender()) 
		//	{
		//		objVect2->push_back(obj);
		//	}
		//	else 
		//	{
		//		objVect3->push_back(obj);
		//	}
		//}

		////implementation idea #2
		//for (unsigned int i = 0; i < objVect1->size(); ++i) 
		//{
		//	Object *obj = (*objVect1)[i];
		//	if (!obj->first)
		//	{
		//		//erase the render and update the position
		//		obj->erase();
		//		obj->update();
		//	}
		//	else
		//	{
		//		obj->first = false;
		//	}
		//	if (obj->notOutOfBound()) {
		//		//obj to be tested for collision
		//		objVect2->push_back(obj);
		//	}
		//	else
		//	{
		//		//obj to be thrown away
		//		objVect3->push_back(obj);
		//	}
		//}
		
		//implementation idea #3 with tree
		qNode->updateObject();

		//Update tree here:
		qNode->updateQuadtree();

		//Collision for objects are done with projectile collision

		//TODO: Instead of rendering from objVect, render by recurrsing through the Quadtree.
		qNode->renderFromTree();

		if (!player->collisionDetection(qNode))
		{
			player->render();
		}
		else {
			//GAME OVER!
			isGameOver = true;
		}

		//swap and clear vectors. Needed before collision testing is implementated
		//objVect1->swap(*objVect2);
		//objVect2->clear();

		////render last from objVect1
		//for (unsigned int i = 0; i < objVect1->size(); ++i)
		//{
		//	(*objVect1)[i]->render();
		//}

		////free objects out of bound
		//for (std::vector<Object *>::iterator it = objVect3->begin(); it != objVect3->end(); ++it) 
		//{
		//	delete *it;
		//}
		//objVect3->clear();

		objMtx.unlock();

		Sleep(100);
	}
	gameOver();
}

void createQuadtree(QuadtreeNode* node) 
{
	if ((node->xMax - node->xMin > 12) && (node->yMax - node->yMin > 8)) 
	{
		node->createSubNodes();
		createQuadtree(node->nw);
		createQuadtree(node->ne);
		createQuadtree(node->sw);
		createQuadtree(node->se);
	}
}

void initQNode(QuadtreeNode *node) 
{
	//maybe make this not hardcoded
	node->xMin = 0;
	node->xMax = 80;
	node->yMin = 0;
	node->yMax = 71;
	node->nodeObjectVector = new std::vector<Object *>();
}

int main() 
{
	HWND console = GetConsoleWindow();
	MoveWindow(console, 0, 0, 800, 1200, TRUE);

	finalScore = 0;

	qNode = new QuadtreeNode();
	initQNode(qNode);
	createQuadtree(qNode);

	player = new Player();
	player->spawn();

	//player->updateAndRender();
	player->erase();
	player->update();
	if (player->collisionDetection(qNode))
	{
		player->render();
	}

	//create thread to monitor keyboard press
	std::thread KeyPressThread(monitorKB, player);
	std::thread monitorProjThread(monitorProj);
	std::thread setupThread(setup);

	KeyPressThread.join();
	monitorProjThread.join();
	setupThread.join();
	delete &KeyPressThread;
	delete &monitorProjThread;
	delete &setupThread;
	return 0;
}