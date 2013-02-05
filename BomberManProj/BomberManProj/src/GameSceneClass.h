#pragma once
#include <Ogre.h>
#include "PlayerClass.h"
class GameSceneClass
{
public:
	GameSceneClass(void);
	~GameSceneClass(void);

	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;

	Ogre::AnimationState *animState;
	Ogre::SceneNode *testNode;

	void createSceneMgr(Ogre::Root *_root);
	void createCamera(void);
	void createScene(void);

	void animation();

};

