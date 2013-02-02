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

	mCamera->setPosition(Ogre::Vector3(0,1000,1000));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
}


void MenuSceneClass::createScene(void)
{
	// title
	Ogre::Entity* title = mSceneMgr->createEntity("boomb_title", "boomb_title.mesh");
	title->setMaterialName("boomb_title");
	Ogre::SceneNode* titleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	titleNode->setPosition(0, 750, 700);
	Ogre::Quaternion q1 = titleNode->getOrientation();
	Ogre::Quaternion q2(Ogre::Degree(45), Ogre::Vector3::UNIT_X);
	titleNode->setOrientation(q1*q2);
	titleNode->attachObject(title);

	// background
	Ogre::SceneNode *backgroundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	backgroundNode->setPosition(0, 0, 0);

	Ogre::Entity *building = mSceneMgr->createEntity("boomb_building", "boomb_building.mesh");
	Ogre::SceneNode *buildingNode = backgroundNode->createChildSceneNode();
	buildingNode->setPosition(500, 0, 200);
	buildingNode->setScale(500,500,500);
	q1 = buildingNode->getOrientation();
	q2 = Ogre::Quaternion(Ogre::Degree(45), Ogre::Vector3::UNIT_Y);
	buildingNode->setOrientation(q1*q2);
	buildingNode->attachObject(building);

	// Set Fog
	Ogre::ColourValue fadeColour(1,0.9,0.7);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 500, 1500);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("directlight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(Ogre::Vector3(-1, -1, -1));

}