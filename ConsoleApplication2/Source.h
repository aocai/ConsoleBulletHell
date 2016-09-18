#pragma once
#include <vector>
#include <mutex>
#include "windows.h"

class Projectile;
class QuadtreeNode;
class Player;

extern std::vector<Projectile *> *renderProjVector;

extern const int NODE_MIN_WIDTH;
extern const int NODE_MIN_HEIGHT;
extern const int MAX_HEIGHT;
extern const int MAX_WIDTH;

extern int finalScore;
extern HANDLE handle;
extern std::mutex consoleMtx;
extern std::mutex projMtx;
extern QuadtreeNode *qNode;

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