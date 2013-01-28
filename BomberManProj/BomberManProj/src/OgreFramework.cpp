#include "OgreFramework.h"

#include <OgreException.h>
#include <OgreConfigFile.h>

OgreFramework::OgreFramework(void)
	:mRoot(0),
	menuScene(0),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mResourcesCfg(Ogre::StringUtil::BLANK)
{
}


OgreFramework::~OgreFramework(void)
{
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
	mWindow = mRoot->initialise(true, "BomberManProj");

	// set default mipmap level
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	// initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	menuScene = MenuSceneClass(mRoot);
	menuScene.createSceneMgr();
	menuScene.createCamera();
	menuScene.createViewport(mWindow);
	menuScene.createScene();
	menuScene.createFrameListener();
	
	
	mRoot->startRendering();

	return true;
}