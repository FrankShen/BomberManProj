#pragma once
#define MAP_NONE 0
#define MAP_NPC 1
#define MAP_NON_NPC 2
#define MAP_BOMB 5
#define MAP_DESTROYABLE 4
#define MAP_INDESTROYABLE 3
#include <Ogre.h>
#include <fstream>
class MapClass
{
public:
	MapClass(void);
	~MapClass(void);
	int getMapAtPos(int x, int y);
	void setMapAtPos(int x, int y, int mapType);
	Ogre::Vector2 nonNPCStartPos;
	Ogre::Vector2 NPCStartPos;
private:
	int data[17][15];
};

