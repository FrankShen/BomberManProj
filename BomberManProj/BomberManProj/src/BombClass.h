#pragma once
#define NON_NPC_BOMB 1001
#define NPC_BOME 1002
#include <Ogre.h>
class BombClass
{
public:
	BombClass(void);
	~BombClass(void);
	
	Ogre::SceneNode *node;

	int bombType;
	int power;
	float countDown;
	Ogre::Vector2 pos;
};

