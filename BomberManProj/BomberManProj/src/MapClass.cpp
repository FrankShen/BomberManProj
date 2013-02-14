#include "MapClass.h"


MapClass::MapClass(void)
{

	std::ifstream file;
	file.open("map.txt");
	if (!file){
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
	} else {
		for (int j = 0; j < 15; ++j){
			for (int i = 0; i < 17; ++i){
				if (i == 0 || i == 16 || j == 0 || j == 14){
					data[i][j] = MAP_INDESTROYABLE;
				} else {
					file >> data[i][j];
					if (data[i][j] == MAP_NON_NPC){
						data[i][j] = MAP_NONE;
						nonNPCStartPos.x = i - 1;
						nonNPCStartPos.y = j - 1;
					} else if (data[i][j] == MAP_NPC){
						data[i][j] = MAP_NONE;
						NPCStartPos.x = i - 1;
						NPCStartPos.y = j - 1;
					}
				}
			}
		}
	}
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