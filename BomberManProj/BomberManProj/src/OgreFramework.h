#pragma once
#include <Ogre.h>
#include "MenuSceneClass.h"

class OgreFramework :
	public Ogre::FrameListener,
	public Ogre::WindowEventListener
{
public:
	OgreFramework(void);
	~OgreFramework(void);
	bool initOgre(void);


	// FrameListener
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// WindowEventListener
	void windowClosed(Ogre::RenderWindow* rw);

private:
	Ogre::Root *mRoot;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow *mWindow;
	Ogre::Viewport *mViewport;
	OIS::InputManager *mInputMgr;
	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;
	MenuSceneClass menuScene;

	void createMenuScene(void);

};

