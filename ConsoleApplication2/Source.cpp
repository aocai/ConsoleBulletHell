#include <stdlib.h>
#include <stdio.h>
#include "windows.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <thread>
#include "Source.h"
#include <time.h>

#define _WIN32_WINNT 0x500

bool isGameOver;
int NODE_MIN_WIDTH = 12;
int NODE_MIN_HEIGHT = 8;
int SIZE_ZERO = 0;
int MAX_HEIGHT = 71;
int MAX_WIDTH = 80;

//clear console by printing empty strings
void clearConsole()
{
	std::string emptyString = "                                                                                ";

	COORD coord;
	//render new position
	coord.X = 0;
	coord.Y = 0;
	while (coord.Y <= 71) 
	{
		SetConsoleCursorPosition(handle, coord);
		std::cout << emptyString;
		coord.Y++;
	}
}

//monitor start/end screen keyboard input
void interfaceKB()
{
	int KB_code = 0;

	//while ESC not pressed
	while (KB_code != 27) 
	{
		if (_kbhit())
		{
			KB_code = _getch();

			//KB_ENTER
			if (KB_code == 13) 
			{
				//clear previous render
				clearConsole();

				//restart
				setUpGameThread();
			}
		}
	}
}

//monitor Keyboard for movement/projectile input
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
				player->speedX -= 1;
				break;
				//KB_RIGHT
			case 77:
				player->speedX += 1;
				break;
				//KB_UP
			case 72:
				player->speedY -= 1;
				break;
				//KB_DOWN
			case 80:
				player->speedY += 1;
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
				//KB_ESC
			case 27:
				isGameOver = true;
			}

		}
	}
}

//Clear console, free all memory and print end game screen
void gameOver()
{
	//clear quadtree and delete all objects
	qNode->eraseAllObjects();
	
	clearConsole();

	//delete and clear from the three projectile vectors
	for (unsigned int i = 0; i < renderProjVector->size(); ++i)
	{
		(*renderProjVector)[i]->erase();
		delete (*renderProjVector)[i];
	}

	for (unsigned int i = 0; i < deleteProjVector->size(); ++i)
	{
		(*deleteProjVector)[i]->erase();
		delete (*deleteProjVector)[i];
	}

	renderProjVector->clear();
	deleteProjVector->clear();

	//Print end screen
	COORD coord;
	//render new position
	coord.X = 30;
	coord.Y = 30;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "GAME OVER!";
	coord.Y += 1;
	SetConsoleCursorPosition(handle, coord);
	if (finalScore < 100) 
	{
		std::cout << "Only " << finalScore << "? Pft are you even trying";
	}
	else if (finalScore < 250) 
	{
		std::cout << "I guess " << finalScore << " is decent...";
	}
	else if (finalScore > 500) 
	{
		std::cout << "Wow " << finalScore << "... dont you have anything better to do?";
	}
	coord.Y += 1;
	SetConsoleCursorPosition(handle, coord);
	std::cout << "PRESS ENTER TO RESTART!";
	coord.Y += 1;
	SetConsoleCursorPosition(handle, coord);
}


void updateProjectile()
{
	//erase all current projectile before next frame
	for (unsigned int i = 0; i < renderProjVector->size(); ++i)
	{
		Projectile *proj = (*renderProjVector)[i];
		if (!proj->first)
		{
			//erase the render and update the position
			proj->erase();
			if (!proj->collision)
			{
				proj->update();
			}
		}
		else
		{
			proj->first = false;
		}
		if (proj->notOutOfBound()) 
		{
			//keep in projVect1, do nothing
		}
		else
		{
			//proj to be thrown away
			deleteProjVector->push_back(proj);
			std::swap((*renderProjVector)[i], renderProjVector->back());
			renderProjVector->pop_back();
			--i;
		}
	}
}

void setup()
{
	while (!isGameOver)
	{
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

		//Update tree to reflect all object position change
		qNode->updateQuadtree();

		//render object with updated position
		qNode->renderFromTree();

		//erase previous projectile render and update all projectile position
		projMtx.lock();
		updateProjectile();
		projMtx.unlock();

		//render all projectiles. delete projectiles that have collision flagged
		projMtx.lock();
		for (unsigned int i = 0; i < renderProjVector->size(); ++i)
		{
			if ((*renderProjVector)[i]->collision)
			{
				//collision flagged. Add to projVect3 to be deleted
				deleteProjVector->push_back((*renderProjVector)[i]);
				std::swap((*renderProjVector)[i], renderProjVector->back());
				renderProjVector->pop_back();
				--i;
			}
			else
			{
				//render
				(*renderProjVector)[i]->render();
			}
		}
		projMtx.unlock();

		//update and render player
		player->erase();
		player->update();
		player->render();

		projMtx.lock();
		//check collision. Both objects and projectiles that have collided are flagged
		for (unsigned int i = 0; i < renderProjVector->size(); ++i)
		{
			Projectile *proj = (*renderProjVector)[i];
			proj->collisionTest(qNode);
		}

		//check for player collision, and flag if true
		player->collisionDetection(qNode);
		if (player->collision)
		{
			isGameOver = true;
		}

		//free non-rendered projectiles
		for (unsigned int i = 0; i < deleteProjVector->size(); ++i)
		{
			delete (*deleteProjVector)[i];
		}

		deleteProjVector->clear();
		projMtx.unlock();

		Sleep(50);
	}
	gameOver();
}

//create initial Quadtree, with size restriction
void createQuadtree(QuadtreeNode* node) 
{
	//bounds check
	if ((node->maxX - node->minX > NODE_MIN_WIDTH) && (node->maxY - node->minY > NODE_MIN_HEIGHT)) 
	{
		node->createSubNodes();
		createQuadtree(node->nw);
		createQuadtree(node->ne);
		createQuadtree(node->sw);
		createQuadtree(node->se);
	}
}

//initialize tree with max boundaries
void initQNode(QuadtreeNode *node) 
{
	node->minX = SIZE_ZERO;
	node->maxX = MAX_WIDTH;
	node->minY = SIZE_ZERO;
	node->maxY = MAX_HEIGHT;
	node->nodeObjectVector = new std::vector<Object *>();
}

//set up game. called at game start
void setUpGameThread()
{
	isGameOver = false;
	finalScore = 0;
	player->spawn();
	player->render();
	std::thread setupThread(setup);
	std::thread KeyPressThread(monitorKB, player);

	setupThread.join();
	KeyPressThread.join();
}

int main() 
{
	HWND console = GetConsoleWindow();
	MoveWindow(console, 0, 0, 800, 1200, TRUE);

	//create Quadtree
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

	//Game has ended. Free quadtree
	qNode->freeQuadtree();
	delete qNode;

	return 0;
}


/*
Bug list:

Make object spawn rate increase throughout the game.
>possibily add health to objects so they are not ohko'able

Add more objects and projectiles. Possibily make objects able to fire projectiles
>add cooldowns

*/

/*
Fixed/looked into:
>Figure out if hard coding the size of window is necessary. Also change the width/height of the window to not hardcoded but as a function of front size. Also see if front size can be set for compatibility across comps.

>No collision if player moving downward. Speeding in opposite direction (beside going up) problem.

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