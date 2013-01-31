#pragma once
#include <Ogre.h>
#include <OIS.h>

class MenuSceneClass
{
public:
	MenuSceneClass(void);
	~MenuSceneClass(void);

	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;

	void createSceneMgr(Ogre::Root *_root);
	void createCamera(void);
	void createScene(void);


};

