#pragma once
#include <Ogre.h>
#include "MenuSceneClass.h"

class OgreFramework
{
public:
	OgreFramework(void);
	~OgreFramework(void);
	bool initOgre(void);

private:
	Ogre::Root *mRoot;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow *mWindow;
	MenuSceneClass menuScene;
};

