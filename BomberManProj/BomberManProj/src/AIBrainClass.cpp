#include "AIBrainClass.h"
#include <iostream>

AIBrainClass::AIBrainClass(void)
{
}


AIBrainClass::~AIBrainClass(void)
{
}

int AIBrainClass::calculateNPCNextEvent(PlayerClass &nonNPC, PlayerClass &npc, MapClass &map)
{
	for (int x = 0; x < 15; ++x){
		for (int y = 0; y < 13; ++y){
			if (map.getMapAtPos(x, y) >= 3) {
				mapFlood[x][y] = -1;
			} else {
				mapFlood[x][y] = 0;
			}
		}
	}
	if (distance(nonNPC.pos.x, nonNPC.pos.y, npc.pos.x, npc.pos.y) <= 2){
		return AI_WAIT;
	}


	calculateRecursive(npc.pos.x, npc.pos.y, 1);


	return traceBack(nonNPC.pos.x, nonNPC.pos.y);
}

void AIBrainClass::calculateRecursive(int x, int y, int level)
{
	if (x < 0 || y < 0 || x > 14 || y > 12){
		return;
	}
	mapFlood[x][y] = level;

	if (getValue(x-1,y) != -1){
		if (getValue(x-1,y) == 0){
			calculateRecursive(x-1, y, level+1);
		} else if (getValue(x-1,y) > (level+1)){
			calculateRecursive(x-1, y, level+1);
		}
	}
	if (getValue(x,y-1) != -1){
		if (getValue(x,y-1) == 0){
			calculateRecursive(x, y-1, level+1);
		} else if (getValue(x,y-1) > (level+1)){
			calculateRecursive(x, y-1, level+1);
		}
	}
	if (getValue(x+1,y) != -1){
		if (getValue(x+1,y) == 0){
			calculateRecursive(x+1, y, level+1);
		} else if (getValue(x+1,y) > (level+1)){
			calculateRecursive(x+1, y, level+1);
		}
	}
	if (mapFlood[x][y+1] != -1){
		if (mapFlood[x][y+1] == 0){
			calculateRecursive(x, y+1, level+1);
		} else if (mapFlood[x][y+1] > (level+1)){
			calculateRecursive(x, y+1, level+1);
		}
	}
}

void AIBrainClass::output()
{
	for (int y = 0; y < 13; ++y){
		for (int x = 0; x < 15; ++x){
			std::cout << mapFlood[x][y] << " ";

		}
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl << std::endl;
}

int AIBrainClass::getValue(int x, int y)
{
	if (x >= 0 && x <= 14 && y >= 0 && y <= 12)
		return mapFlood[x][y];
	else
		return -1;
}

int AIBrainClass::traceBack(int _x, int _y)
{
	int destinationLevel = mapFlood[_x][_y];
	int x = _x;
	int y = _y;
	for (; destinationLevel >= 3; --destinationLevel){
		if (getValue(x-1, y) == (destinationLevel-1)){
			x = x-1;
			continue;
		}
		if (getValue(x+1, y) == (destinationLevel-1)){
			x = x+1;
			continue;
		}
		if (getValue(x, y-1) == (destinationLevel-1)){
			y = y-1;
			continue;
		}
		if (getValue(x, y+1) == (destinationLevel-1)){
			y = y+1;
			continue;
		}
	}
	if (getValue(x-1,y) == 1){
		return AI_RIGHT;
	}
	if (getValue(x+1,y) == 1){
		return AI_LEFT;
	}
	if (getValue(x,y-1) == 1){
		return AI_DOWN;
	}
	if (getValue(x,y+1) == 1){
		return AI_UP;
	}
	return AI_WAIT;
}

int AIBrainClass::distance(int x1, int y1, int x2, int y2)
{
	return (abs(x1-x2)+abs(y1-y2));
}