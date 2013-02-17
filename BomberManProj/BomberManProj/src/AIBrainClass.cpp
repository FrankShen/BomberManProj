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
			} else if (map.getMapAtPos(x, y) == MAP_NON_NPC){
				mapFlood[x][y] = -2;
			} else {
				mapFlood[x][y] = 0;
			}
		}
	}
	return calculateRecursive(npc.pos.x, npc.pos.y, 1, 0);
}

int AIBrainClass::calculateRecursive(int x, int y, int level,int direction)
{
	if (x < 0 || x > 14 || y < 0 || y > 12){
		return 0;
	}

	if (mapFlood[x][y] == 0){
		mapFlood[x][y] = level;
	} else if (mapFlood > 0){
		if (mapFlood[x][y] > level){
			mapFlood[x][y] = level;
		} else {
			return 0;
		}
	} else {
		return 0;
	}

	if (mapFlood[x-1][y] == -2 && direction != DIR_LEFT){
		output();
		return AI_LEFT;
	}
	if (mapFlood[x][y-1] == -2  && direction != DIR_UP ){
		return AI_UP;
	}
	if (mapFlood[x+1][y] == -2  && direction != DIR_RIGHT){
		return AI_RIGHT;
	}
	if (mapFlood[x][y+1] == -2  && direction != DIR_DOWN){
		return AI_DOWN;
	}

	if (direction != DIR_LEFT){
		if (calculateRecursive(x-1, y, level+1, DIR_LEFT)){
			return AI_LEFT;
		}
	}
	if (direction != DIR_UP){
		if (calculateRecursive(x, y-1, level+1, DIR_UP)){
			return AI_UP;
		}
	}
	if (direction != DIR_RIGHT){
		if (calculateRecursive(x+1, y, level+1, DIR_RIGHT)){
			return AI_RIGHT;
		}
	}
	if (direction != DIR_DOWN){
		if (calculateRecursive(x, y+1, level+1, DIR_DOWN)){
			return AI_DOWN;
		}
	}
	return 0;
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