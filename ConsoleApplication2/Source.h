#pragma once
#include <vector>
#include <mutex>
#include "windows.h"
#include "Projectile.h"
#include "Player.h"
#include "Tri.h"
#include "Circ.h"
#include "QuadtreeNode.h"

std::vector<Projectile *> *renderProjVector = new std::vector<Projectile *>(); //all projectile vectors
std::vector<Projectile *> *deleteProjVector = new std::vector<Projectile *>(); //projectile vectors to keep

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
std::mutex consoleMtx;
std::mutex projMtx;
QuadtreeNode *qNode;
Player *player;
int finalScore;

void clearConsole();
void interfaceKB();
void monitorKB(Player *player);
void setup();
void gameOver();
void updateProjectile();
void initQNode(QuadtreeNode *qNode);
void createQuadtree(QuadtreeNode *qNode);
void setUpGameThread();
int main();