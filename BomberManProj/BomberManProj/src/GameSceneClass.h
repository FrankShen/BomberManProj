#pragma once
#include <Ogre.h>
#include "PlayerClass.h"
#include "PlayerClass.h"

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

	PlayerClass nonNPCPlayer;
	PlayerClass NPCPlayer;

	void createSceneMgr(Ogre::Root *_root);
	void createCamera(void);
	void createScene(void);

	void askingPlayer(int playerType, int eventType);

private:
	void movingPlayer(int playerType, Ogre::SceneNode *playerNode, Ogre::Vector3 direction, Ogre::AnimationState *animState);

};

