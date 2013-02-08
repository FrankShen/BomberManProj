#include "MapClass.h"


MapClass::MapClass(void)
{
	for (int i = 0; i < 17; ++i){
		for (int j = 0; j < 15; ++j){
			if (i == 0 || i == 16 || j == 0 || j == 14){
				data[i][j] = MAP_INDESTROYABLE;
			} else {
				data[i][j] = MAP_NONE;
			}
		}
	}
	nonNPCStartPos.x = 3;
	nonNPCStartPos.y = 4;
	NPCStartPos.x = 14;
	NPCStartPos.y = 12;
}


MapClass::~MapClass(void)
{
}

int MapClass::getMapAtPos(int x, int y)
{
	return data[x+1][y+1];
}

void MapClass::setMapAtPos(int x, int y, int mapType)
{
	data[x+1][y+1] = mapType;
}