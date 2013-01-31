#include "MenuSceneClass.h"

MenuSceneClass::MenuSceneClass(void)
{

}


MenuSceneClass::~MenuSceneClass(void)
{
}

void MenuSceneClass::createSceneMgr(Ogre::Root *_root)
{
	mSceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "MenuSceneMgr");
}

void MenuSceneClass::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("MenuCamera");

	mCamera->setPosition(Ogre::Vector3(100,80,100));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
}

void MenuSceneClass::createScene(void)
{
	Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
	/* Cel-Shade
	ogreHead->setMaterialName("Examples/CelShading");
	Ogre::SubEntity* sub;

	sub = ogreHead->getSubEntity(0);    // eyes
	sub->setCustomParameter(1, Ogre::Vector4(35, 0, 0, 0));
	sub->setCustomParameter(2, Ogre::Vector4(1, 0.3, 0.3, 1));
	sub->setCustomParameter(3, Ogre::Vector4(1, 0.6, 0.6, 1));

	sub = ogreHead->getSubEntity(1);    // skin
	sub->setCustomParameter(1, Ogre::Vector4(10, 0, 0, 0));
	sub->setCustomParameter(2, Ogre::Vector4(0, 0.5, 0, 1));
	sub->setCustomParameter(3, Ogre::Vector4(0.3, 0.5, 0.3, 1));

	sub = ogreHead->getSubEntity(2);    // earring
	sub->setCustomParameter(1, Ogre::Vector4(25, 0, 0, 0));
	sub->setCustomParameter(2, Ogre::Vector4(1, 1, 0, 1));
	sub->setCustomParameter(3, Ogre::Vector4(1, 1, 0.7, 1));

	sub = ogreHead->getSubEntity(3);    // teeth
	sub->setCustomParameter(1, Ogre::Vector4(20, 0, 0, 0));
	sub->setCustomParameter(2, Ogre::Vector4(1, 1, 0.7, 1));
	sub->setCustomParameter(3, Ogre::Vector4(1, 1, 1, 1));
	*/

	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	headNode->attachObject(ogreHead);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

}