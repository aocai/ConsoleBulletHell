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

bool isGameOver;

void clearConsole()
{
	std::string emptyString = "                                                                                ";
	COORD coord;
	//render new position
	coord.X = 0;
	coord.Y = 0;
	while (coord.Y <= 71) {
		SetConsoleCursorPosition(handle, coord);
		std::cout << emptyString;
		coord.Y++;
	}
}

void interfaceKB()
{
	int KB_code = 0;
	while (KB_code != 27) {
		if (_kbhit())
		{
			KB_code = _getch();
			if (KB_code == 13) {
				//KB_ENTER

				clearConsole();

				//restart
				setUpGameThread();
			}
		}
	}
}


void monitorKB(Player *player)
{
	int KB_code = 0;
	//while (KB_code != 27) 
	while (!isGameOver)
	{
		if (_kbhit())
		{
			KB_code = _getch();
			switch (KB_code)
			{
				//KB_LEFT
			case 75:
				player->speedX = -1;
				player->speedY = 0;

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
				player->speedX = 1;
				player->speedY = 0;

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
			case 27:
				isGameOver = true;
				gameOver();
			}

		}
	}
}

void monitorProj() 
{
	while (!isGameOver)
	{
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

		projMtx.unlock();

		objMtx.lock();

		//update tree
		qNode->updateQuadtree();

		objMtx.unlock();

		projMtx.lock();

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

		for (unsigned int i = 0; i < projVect3->size(); ++i)
		{
			delete (*projVect3)[i];
		}

		projVect3->clear();

		projMtx.unlock();
		
		Sleep(50);
	}
}


void gameOver()
{
	player->erase();

	//erase render
	qNode->updateObject();
	//clear quadtree
	qNode->eraseAllObjects();

	for (unsigned int i = 0; i < projVect1->size(); ++i)
	{
		(*projVect1)[i]->erase();
		delete (*projVect1)[i];
	}

	for (unsigned int i = 0; i < projVect2->size(); ++i)
	{
		(*projVect2)[i]->erase();
		delete (*projVect2)[i];
	}

	for (unsigned int i = 0; i < projVect3->size(); ++i)
	{
		(*projVect3)[i]->erase();
		delete (*projVect3)[i];
	}

	projVect1->clear();
	projVect2->clear();
	projVect3->clear();


	COORD coord;
	//render new position
	coord.X = 30;
	coord.Y = 30;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "GAME OVER!";
	coord.Y += 1;
	SetConsoleCursorPosition(handle, coord);
	if (finalScore < 100) {
		std::cout << "Only " << finalScore << "? Pft are you even trying";
	}
	else if (finalScore < 250) {
		std::cout << "I guess " << finalScore << "is decent...";
	}
	else if (finalScore > 500) {
		std::cout << "Wow " << finalScore << "... dont you have anything better to do?";
	}
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
		}
		if (rand() % 5 == 0) 
		{
			Object *circ = new Circ();
			circ->spawn();
			qNode->assignQNode(circ);
		}
		
		//erase previous object render and update all object position

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
		
		objMtx.unlock();

		Sleep(100);
	}
	gameOver();
}

void createQuadtree(QuadtreeNode* node) 
{
	if ((node->maxX - node->minX > 12) && (node->maxY - node->minY > 8)) 
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
	node->minX = 0;
	node->maxX = 80;
	node->minY = 0;
	node->maxY = 71;
	node->nodeObjectVector = new std::vector<Object *>();
}

void setUpGameThread()
{
	isGameOver = false;
	finalScore = 0;
	player->spawn();
	player->render();
	std::thread setupThread(setup);
	std::thread KeyPressThread(monitorKB, player);
	std::thread monitorProjThread(monitorProj);

	setupThread.join();
	KeyPressThread.join();
	monitorProjThread.join();
}

int main() 
{
	HWND console = GetConsoleWindow();
	MoveWindow(console, 0, 0, 800, 1200, TRUE);

	qNode = new QuadtreeNode();
	initQNode(qNode);
	createQuadtree(qNode);

	player = new Player();

	COORD coord;
	//render new position
	coord.X = 30;
	coord.Y = 30;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "Press ENTER to start!";

	std::thread interfaceKBThread(interfaceKB);

	interfaceKBThread.join();


	return 0;
}


/*
Bug list:

Figure out if hard coding the size of window is necessary. Also change the width/height of the window to not hardcoded but as a function of front size. Also see if front size can be set for compatibility across comps.

Make object spawn rate increase throughout the game.
>possibily add health to objects so they are not ohko'able

Add more objects and projectiles. Possibily make objects able to fire projectiles

*/

/*
Fixed/looked into:

>Replace all loops using .size() with iterators...

>Make start screen

>Implement restart game function from game over

>Make stuff not hardcoded, aka height

>Make objects/projectiles not use height/width, but rather xMax,xMin,yMax,yMin
>if this is implemented, fix the posX posY inconsistency between objects/nodes and projectiles

>Find a way to erase all updateAndRender() calls

>circ object sometimes go through player without game ending
>might be an issue with objects that had collided with a projectile not deleted properly, and so it passes through player
>found to be a issue with two threads running different Sleep timer

>Change the abstract class to be compact (remove all unnecessary variables/functions)

>Fix the inconsistency between thread calls, with one having Sleep(50) and one having Sleep(100)
>not necessary

>Use level by level recursion instead of starting from root node when doing reassignQNode() when you need to go back up a level
>not ideal for small trees

Make collision detection not just use current position, but current and previous
>Collision detection seems clunky, look into whether should be using "future detection" instead of current detection
>currently theres a frame where both object and projectile intersect, and is rendered like so
>actually, with the object being erased at collision rather than object update, it looks way smoother now, so this is not necessary
*/