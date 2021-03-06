#pragma once
#include <Ogre.h>
#include <map>
#include <audiere.h>
#include "PlayerClass.h"
#include "AIBrainClass.h"
#include "MapClass.h"
#include "BombClass.h"
#define GRID_SIZE 70
#define DEFAULT_SPEED 0.3

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

	AIBrainClass aiBrain;

	std::map<int, BombClass> bombPool;

	bool isSpaceKeyDown;

	void initGameData(void);
	void createSceneMgr(Ogre::Root *_root);
	void createCamera(void);
	void createScene(void);

	void askingPlayer(int playerType, int eventType);
	void updatePlayerPos(void);
	void updateNPCPlayerPos(void);

	int thromBomb(PlayerClass &player);
	void updateBombInfo(const Ogre::FrameEvent& evt);

	int updatePlayerInfo(const Ogre::FrameEvent& evt);

private:

	audiere::OutputStreamPtr explodeSound;

	int bombIndex;
	Ogre::Vector3 getWorldCoord(Ogre::Vector2 pos);
	void movingPlayer(int playerType, float speed, Ogre::SceneNode *playerNode, Ogre::Vector3 direction, Ogre::AnimationState *animState);
	void calculateBombArea(BombClass &bomb);
	void explodeBomb(int bombIdx);
	void addBonus(Ogre::Vector2 pos);

};

