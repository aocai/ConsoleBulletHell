#pragma once
#include <vector>
#include <mutex>
#include "windows.h"
#include "Projectile.h"
#include "Player.h"
#include "Tri.h"
#include "Circ.h"
#include "QuadtreeNode.h"

std::vector<Projectile *> *projVect1 = new std::vector<Projectile *>(); //all projectile vectors
std::vector<Projectile *> *projVect2 = new std::vector<Projectile *>(); //projectile vectors to keep
std::vector<Projectile *> *projVect3 = new std::vector<Projectile *>(); //projectile vectors to delete

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
std::mutex consoleMtx;
std::mutex objMtx;
std::mutex projMtx;
QuadtreeNode *qNode;
Player *player;
int finalScore;

void clearConsole();
void interfaceKB();
void monitorProj();
void monitorKB(Player *player);
void setup();
void gameOver();
void initQNode(QuadtreeNode *qNode);
void createQuadtree(QuadtreeNode *qNode);
void setUpGameThread();
int main();