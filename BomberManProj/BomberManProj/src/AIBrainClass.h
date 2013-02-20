#pragma once
#define AI_UP 1001
#define AI_DOWN 1002
#define AI_LEFT 1003
#define AI_RIGHT 1004
#define AI_BOMB	1005
#define AI_WAIT 1006

#define AI_STAT_FARTHER 1001
#define AI_STAT_NPC 1002
#define AI_STAT_WAIT 1003

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
	
	int AIState;
	void calculateRecursive(int x, int y, int level);
	void output();
	int farthestLevel;
	int farthestX;
	int farthestY;
	int npcPosX;
	int npcPosY;
	void getFarthestInfo(void);
	int getValue(int x, int y);
	int traceBack(int _x, int _y);
	int distance(int x1, int y1, int x2, int y2);
};

