#pragma once
#include <Ogre.h>
#include <OIS.h>
class MenuSceneClass :
	public Ogre::FrameListener
{
public:
	MenuSceneClass(Ogre::Root *_root);
	~MenuSceneClass(void);

	Ogre::Root *mRoot;
	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;
	Ogre::Viewport *mVp;
	Ogre::RenderWindow *mWindow;

	void createSceneMgr(void);
	void createCamera(void);
	void createViewport(Ogre::RenderWindow *_Wnd);
	void createFrameListener(void);
	void createScene(void);

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

};

