#pragma once
#include <Ogre.h>
#include <OIS.h>
#include <audiere.h>
#include "MenuSceneClass.h"
#include "GameSceneClass.h"

#define MENUSCENE 1001
#define GAMESCENE 1002

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
	int sceneState;

	Ogre::Root *mRoot;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow *mWindow;
	Ogre::Viewport *mViewport;
	OIS::InputManager *mInputMgr;
	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;
	MenuSceneClass menuScene;
	GameSceneClass gameScene;

	bool mShutdown;

	void createMenuScene(void);
	void createGameScene(void);
	void logicalFrameFunc(const Ogre::FrameEvent& evt);
	void gameStart(void);
	void gameEnd(void);
};

