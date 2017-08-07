#include "windows.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <thread>
#include "Source.h"
#include "Player.h"
#include "QuadtreeNode.h"
#include "Projectile.h"
#include "Tri.h"
#include "Circ.h"
#include <time.h>

bool isGameOver;

std::vector<Projectile *> renderProjVector;
std::vector<Projectile *> deleteProjVector;

const int NODE_MIN_WIDTH = 12;
const int NODE_MIN_HEIGHT = 8;
const int MAX_HEIGHT = 80;
const int MAX_WIDTH = 80;
int finalScore = 0;
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
QuadtreeNode *qNode = new QuadtreeNode();
std::mutex consoleMtx;
std::mutex projMtx;

//clear console by printing empty strings
void clearConsole()
{
	std::string emptyString = "                                                                                ";

	COORD coord;
	//render new position
	coord.X = 0;
	coord.Y = 0;
	while (coord.Y <= MAX_HEIGHT)
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
void monitorKB()
{
	time_t z = clock();
	time_t x = clock();
	time_t space = clock();
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
				Player::getInstance().speedX -= 1;
				break;
				//KB_RIGHT
			case 77:
				Player::getInstance().speedX += 1;
				break;
				//KB_UP
			case 72:
				Player::getInstance().speedY -= 1;
				break;
				//KB_DOWN
			case 80:
				Player::getInstance().speedY += 1;
				break;
				//KB_Z
			case 122:
			case 90:
				z = clock() - z;
				if ((float)z / CLOCKS_PER_SEC > 1)
				{
					Player::getInstance().spawnNormalAtk();
					z = clock();
				}
				break;
				//KB_X
			case 120:
			case 88:
				x = clock() - x;
				if ((float)x / CLOCKS_PER_SEC > 1)
				{
					Player::getInstance().spawnShortBeam();
					x = clock();
				}
				break;
				//KB_SPACE
			case 32:
				space = clock() - space;
				if ((float)space / CLOCKS_PER_SEC > 60)
				{
					Player::getInstance().ult();
					space = clock();
				}
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
	for (unsigned int i = 0; i < renderProjVector.size(); ++i)
	{
		renderProjVector[i]->erase();
		delete renderProjVector[i];
	}

	for (unsigned int i = 0; i < deleteProjVector.size(); ++i)
	{
		deleteProjVector[i]->erase();
		delete deleteProjVector[i];
	}

	renderProjVector.clear();
	deleteProjVector.clear();

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
	for (unsigned int i = 0; i < renderProjVector.size(); ++i)
	{
		Projectile *proj = renderProjVector[i];
		if (!proj->first)
		{
			//erase the render and update the position
			//proj->erase();
			if (!proj->collision)
			{
				proj->update();
			}
		}
		else
		{
			proj->first = false;
		}
		if (!proj->notOutOfBound()) 
		{
			//proj to be thrown away
			deleteProjVector.push_back(proj);
			std::swap(renderProjVector[i], renderProjVector.back());
			renderProjVector.pop_back();
			--i;
		}
	}
}

void setup()
{
	clock_t t;
	clock_t f;
	clock_t fps;
	t = clock();
	f = clock();
	fps = clock();

	while (!isGameOver)
	{
		//random generate objects
		if (rand() % 20 == 0)
		{
			Object *tri = new Tri();
			tri->spawn();
			qNode->assignQNode(tri);
		}
		if (rand() % 20 == 0)
		{
			Object *circ = new Circ();
			circ->spawn();
			qNode->assignQNode(circ);
		}

		t = clock() - t;
		COORD coord;
		coord.X = 0;
		coord.Y = 0;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		//erase previous object render and update all object position
		qNode->updateObject();

		t = clock() - t;
		coord.X = 0;
		coord.Y = 1;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		//Update tree to reflect all object position change
		qNode->updateQuadtree();

		t = clock() - t;
		coord.X = 0;
		coord.Y = 2;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		//erase previous projectile render and update all projectile position
		projMtx.lock();
		updateProjectile();
		projMtx.unlock();

		t = clock() - t;
		coord.X = 0;
		coord.Y = 4;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		//clear console
		system("cls");

		//render object with updated position
		qNode->renderFromTree();

		t = clock() - t;
		coord.X = 0;
		coord.Y = 3;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		//render all projectiles. delete projectiles that have collision flagged
		projMtx.lock();
		for (unsigned int i = 0; i < renderProjVector.size(); ++i)
		{
			if (renderProjVector[i]->collision)
			{
				//collision flagged. Add to projVect3 to be deleted
				deleteProjVector.push_back(renderProjVector[i]);
				std::swap(renderProjVector[i], renderProjVector.back());
				renderProjVector.pop_back();
				--i;
			}
			else
			{
				//render
				renderProjVector[i]->render();
			}
		}
		projMtx.unlock();

		t = clock() - t;
		coord.X = 0;
		coord.Y = 5;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		//update and render player
		Player::getInstance().erase();
		Player::getInstance().update();
		Player::getInstance().render();

		t = clock() - t;
		coord.X = 0;
		coord.Y = 6;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		projMtx.lock();
		//check collision. Both objects and projectiles that have collided are flagged
		for (unsigned int i = 0; i < renderProjVector.size(); ++i)
		{
			Projectile *proj = renderProjVector[i];
			proj->collisionTest(qNode);
		}

		t = clock() - t;
		coord.X = 0;
		coord.Y = 7;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		//check for player collision, and flag if true
		Player::getInstance().collisionDetection(qNode);
		if (Player::getInstance().collision)
		{
			isGameOver = true;
		}

		t = clock() - t;
		coord.X = 0;
		coord.Y = 8;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t / CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		//free non-rendered projectiles
		for (unsigned int i = 0; i < deleteProjVector.size(); ++i)
		{
			delete deleteProjVector[i];
		}

		deleteProjVector.clear();
		projMtx.unlock();

		t = clock() - t;
		coord.X = 0;
		coord.Y = 9;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << ((float)t/CLOCKS_PER_SEC) << "secs";
		consoleMtx.unlock();
		t = clock();

		f = clock() - f;
		double frameTime = (float)f / CLOCKS_PER_SEC;
		Sleep(max(0,(1 / 20.0 - frameTime)*1000));

		fps = clock() - fps;
		coord.X = 0;
		coord.Y = 10;
		consoleMtx.lock();
		SetConsoleCursorPosition(handle, coord);
		std::cout << 1.0/((float)fps / CLOCKS_PER_SEC) << "FPS";
		consoleMtx.unlock();

		f = clock();
		fps = clock();
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
	node->minX = 0;
	node->maxX = MAX_WIDTH;
	node->minY = 0;
	node->maxY = MAX_HEIGHT;
	//node->nodeObjectVector = new std::vector<Object *>();
}

//set up game. called at game start
void setUpGameThread()
{
	isGameOver = false;
	finalScore = 0;
	Player::getInstance().spawn();
	Player::getInstance().render();
	std::thread setupThread(setup);
	std::thread KeyPressThread(monitorKB);

	setupThread.join();
	KeyPressThread.join();
}

int main() 
{
	HWND console = GetConsoleWindow();
	MoveWindow(console, 0, 0, 8 * (MAX_WIDTH + 4), 16 * (MAX_HEIGHT + 4), TRUE);

	//create Quadtree
	initQNode(qNode);
	createQuadtree(qNode);

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
Bug "feature" list:

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