#pragma once
#include <OgreRoot.h>

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
};

