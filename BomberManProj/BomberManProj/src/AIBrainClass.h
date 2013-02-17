#pragma once
#define AI_UP 1001
#define AI_DOWN 1002
#define AI_LEFT 1003
#define AI_RIGHT 1004
#define AI_BOMB	1005
#define AI_WAIT 1006
#define DIR_UP 1001
#define DIR_DOWN 1002
#define DIR_LEFT 1003
#define DIR_RIGHT 1004
#include "PlayerClass.h"
#include "MapClass.h"
class AIBrainClass
{
public:
	AIBrainClass(void);
	~AIBrainClass(void);
	int mapFlood[15][13];
	int calculateNPCNextEvent(PlayerClass &nonNPC, PlayerClass &npc, MapClass &map);
private:
	int calculateRecursive(int x, int y, int level, int direction);
	void output();
};

