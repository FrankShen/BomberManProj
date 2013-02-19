#include "OgreFramework.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>

OgreFramework::OgreFramework(void)
	:mRoot(0),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mKeyboard(0),
	mMouse(0),
	mShutdown(false)
{

}


OgreFramework::~OgreFramework(void)
{
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}

bool OgreFramework::initOgre(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	// construct Ogre::Root
	mRoot = new Ogre::Root(mPluginsCfg);

	// setup resources
	// load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}

	// configure
	// show the configuration dialog and initialise the system
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	// auto create a window called "BomberManProj"

	Ogre::NameValuePairList params;
	params["title"] = "BomberManProj";
	params["border"] = "fixed";
	params["FSAA"] = "8";
	mRoot->initialise(false, "BomberManProj");
	mWindow = mRoot->createRenderWindow("BomberManProj", 800, 600, false, &params);


	// set default mipmap level
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	// initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// initialise OIS
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject( OIS::OISKeyboard, false ));
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject( OIS::OISMouse, false ));

	unsigned int width, height, depth;
	int top, left;
	mWindow->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	// initialise menu scene
	/*
	createMenuScene();
	sceneState = MENUSCENE;
	*/
	createGameScene();
	sceneState = GAMESCENE;

	mRoot->addFrameListener(this);
	mRoot->startRendering();

	return true;
}

void OgreFramework::createMenuScene(void)
{
	menuScene.createSceneMgr(mRoot);
	menuScene.createCamera();
	menuScene.createScene();

	mViewport = mWindow->addViewport(menuScene.mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	menuScene.mCamera->setAspectRatio(
		Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
}

void OgreFramework::createGameScene(void)
{
	gameScene.initGameData();
	gameScene.createSceneMgr(mRoot);
	gameScene.createCamera();
	gameScene.createScene();
	mViewport = mWindow->addViewport(gameScene.mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	gameScene.mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
}
bool OgreFramework::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;
	if (mShutdown)
		return false;
	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	logicalFrameFunc(evt);

	return true;
}

void OgreFramework::windowClosed(Ogre::RenderWindow* rw)
{
	if( rw == mWindow )
	{
		if( mInputMgr )
		{
			mInputMgr->destroyInputObject( mMouse );
			mInputMgr->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputMgr);
			mInputMgr = 0;
		}
	}
}

void OgreFramework::logicalFrameFunc(const Ogre::FrameEvent& evt)
{
	switch(sceneState)
	{
	case MENUSCENE:
		{
			if (mKeyboard->isKeyDown(OIS::KC_RETURN)){
				mShutdown = true;
			}
			menuScene.mTitleAnimState->addTime(evt.timeSinceLastFrame);
			menuScene.mBackgroundAnimState->addTime(evt.timeSinceLastFrame);
			break;
		}
	case GAMESCENE:
		{			
			if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)){
				mShutdown = true;
			}

			if (mKeyboard->isKeyDown(OIS::KC_SPACE)){
				if (!gameScene.isSpaceKeyDown){
					if (gameScene.nonNPCPlayer.bombAvailable && 
						gameScene.map.getMapAtPos(gameScene.nonNPCPlayer.pos.x, gameScene.nonNPCPlayer.pos.y) <= 2){

							gameScene.thromBomb(gameScene.nonNPCPlayer);
					}
					gameScene.isSpaceKeyDown = true;
				}
			} else {
				gameScene.isSpaceKeyDown = false;
			}

			gameScene.updateBombInfo(evt);

			if (mKeyboard->isKeyDown(OIS::KC_DOWN)){
				if (!gameScene.nonNPCAnimState){
					gameScene.askingPlayer(NON_NPC, EVENT_DOWN);
				} else if (gameScene.nonNPCAnimState->hasEnded()){
					gameScene.askingPlayer(NON_NPC, EVENT_DOWN);
				}
			} else if (mKeyboard->isKeyDown(OIS::KC_UP)){
				if (!gameScene.nonNPCAnimState){
					gameScene.askingPlayer(NON_NPC, EVENT_UP);
				} else if (gameScene.nonNPCAnimState->hasEnded()){
					gameScene.askingPlayer(NON_NPC, EVENT_UP);
				}
			} else if (mKeyboard->isKeyDown(OIS::KC_LEFT)){
				if (!gameScene.nonNPCAnimState){
					gameScene.askingPlayer(NON_NPC, EVENT_LEFT);
				} else if (gameScene.nonNPCAnimState->hasEnded()){
					gameScene.askingPlayer(NON_NPC, EVENT_LEFT);
				}
			} else if (mKeyboard->isKeyDown(OIS::KC_RIGHT)){
				if (!gameScene.nonNPCAnimState){
					gameScene.askingPlayer(NON_NPC, EVENT_RIGHT);
				} else if (gameScene.nonNPCAnimState->hasEnded()){
					gameScene.askingPlayer(NON_NPC, EVENT_RIGHT);
				}
			}

			if (!gameScene.NPCAnimState){
				int npcNextEvent = gameScene.aiBrain.calculateNPCNextEvent(gameScene.nonNPCPlayer, gameScene.NPCPlayer, gameScene.map);
				if (npcNextEvent == AI_BOMB){
					if (gameScene.NPCPlayer.bombAvailable && 
						gameScene.map.getMapAtPos(gameScene.NPCPlayer.pos.x, gameScene.NPCPlayer.pos.y) <= 2){

							gameScene.thromBomb(gameScene.NPCPlayer);
					}
				} else if (npcNextEvent == AI_DOWN){
					gameScene.askingPlayer(NPC, EVENT_DOWN);
				} else if (npcNextEvent == AI_UP){
					gameScene.askingPlayer(NPC, EVENT_UP);
				} else if (npcNextEvent == AI_LEFT){
					gameScene.askingPlayer(NPC, EVENT_LEFT);
				} else if (npcNextEvent == AI_RIGHT){
					gameScene.askingPlayer(NPC, EVENT_RIGHT);
				}
			} else if (gameScene.NPCAnimState->hasEnded()){
				int npcNextEvent = gameScene.aiBrain.calculateNPCNextEvent(gameScene.nonNPCPlayer, gameScene.NPCPlayer, gameScene.map);
				if (npcNextEvent == AI_BOMB){
					if (gameScene.NPCPlayer.bombAvailable && 
						gameScene.map.getMapAtPos(gameScene.NPCPlayer.pos.x, gameScene.NPCPlayer.pos.y) <= 2){

							gameScene.thromBomb(gameScene.NPCPlayer);
					}
				} else if (npcNextEvent == AI_DOWN){
					gameScene.askingPlayer(NPC, EVENT_DOWN);
				} else if (npcNextEvent == AI_UP){
					gameScene.askingPlayer(NPC, EVENT_UP);
				} else if (npcNextEvent == AI_LEFT){
					gameScene.askingPlayer(NPC, EVENT_LEFT);
				} else if (npcNextEvent == AI_RIGHT){
					gameScene.askingPlayer(NPC, EVENT_RIGHT);
				}
			}

			gameScene.updatePlayerInfo(evt);
			if (gameScene.nonNPCAnimState){
				if (!gameScene.nonNPCAnimState->hasEnded()){
					gameScene.nonNPCAnimState->addTime(evt.timeSinceLastFrame);
					gameScene.updatePlayerPos();
				}
			} 
			if (gameScene.NPCAnimState){
				if (!gameScene.NPCAnimState->hasEnded()){
					gameScene.NPCAnimState->addTime(evt.timeSinceLastFrame);
					gameScene.updateNPCPlayerPos();
				}
			} 
			break;
		}
	default:
		break;
	}
}