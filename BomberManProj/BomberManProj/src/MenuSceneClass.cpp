#include "MenuSceneClass.h"


MenuSceneClass::MenuSceneClass(Ogre::Root *_root)
{
	mRoot = _root;
	mSceneMgr = NULL;
	mCamera = NULL;
}


MenuSceneClass::~MenuSceneClass(void)
{
}

void MenuSceneClass::createSceneMgr(void)
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "MenuSceneMgr");
}

void MenuSceneClass::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("MenuCamera");

	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);
}

void MenuSceneClass::createViewport(Ogre::RenderWindow *_Wnd)
{
	mWindow = _Wnd;
	mVp = _Wnd->addViewport(mCamera);
	mVp->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	mCamera->setAspectRatio(
		Ogre::Real(mVp->getActualWidth()) / Ogre::Real(mVp->getActualHeight()));
}

void MenuSceneClass::createFrameListener(void)
{
	mRoot->addFrameListener(this);
}

void MenuSceneClass::createScene(void)
{
	Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	headNode->attachObject(ogreHead);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);
}

bool MenuSceneClass::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;
	
	return true;
}