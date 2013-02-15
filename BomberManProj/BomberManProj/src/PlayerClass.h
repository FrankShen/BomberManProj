#pragma once
#include <Ogre.h>
#define NON_NPC 1001
#define NPC 1002
#define EVENT_UP 1003
#define EVENT_DOWN 1004
#define EVENT_LEFT 1005
#define EVENT_RIGHT 1006

class PlayerClass
{
public:
	PlayerClass(void);
	~PlayerClass(void);

	int playerType;
	int bombAvailable;
	int power;
	int health;
	float invincible;
	Ogre::Vector2 pos;
	Ogre::Vector2 destinationPos;
};

