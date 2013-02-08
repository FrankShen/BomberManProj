#pragma once
#include <Ogre.h>
#include <map>
#include "PlayerClass.h"
#include "MapClass.h"
#include "BombClass.h"
#define GRID_SIZE 70
#define DEFAULT_SPEED 0.5

class GameSceneClass
{
public:
	GameSceneClass(void);
	~GameSceneClass(void);

	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;

	Ogre::AnimationState *nonNPCAnimState;
	Ogre::AnimationState *NPCAnimState;

	Ogre::SceneNode *nonNPCPlayerNode;
	Ogre::SceneNode *NPCPlayerNode;
	Ogre::SceneNode *mapNode[15][13];

	MapClass map;
	PlayerClass nonNPCPlayer;
	PlayerClass NPCPlayer;

	std::map<int, BombClass> bombPool;

	bool isSpaceKeyDown;

	void initGameData(void);
	void createSceneMgr(Ogre::Root *_root);
	void createCamera(void);
	void createScene(void);

	void askingPlayer(int playerType, int eventType);
	void updatePlayerPos(void);

	int thromBomb(PlayerClass player);
	void updateBombInfo(const Ogre::FrameEvent& evt);

private:
	int bombIndex;
	Ogre::Vector3 getWorldCoord(Ogre::Vector2 pos);
	void movingPlayer(int playerType, Ogre::SceneNode *playerNode, Ogre::Vector3 direction, Ogre::AnimationState *animState);

};

