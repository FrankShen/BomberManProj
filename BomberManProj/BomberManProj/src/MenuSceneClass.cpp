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
	Ogre::SceneNode* titleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_title");
	titleNode->setPosition(0, 750, 700);
	Ogre::Quaternion q1 = titleNode->getOrientation();
	Ogre::Quaternion q2(Ogre::Degree(45), Ogre::Vector3::UNIT_X);
	titleNode->setOrientation(q1*q2);
	titleNode->attachObject(title);

	// Start text
	Ogre::Entity* startText = mSceneMgr->createEntity("boomb_starttext", "boomb_starttext.mesh");
	Ogre::SceneNode* textNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_starttext");
	textNode->setPosition(0, 800, 900);
	textNode->attachObject(startText);

	// background
	Ogre::SceneNode *backgroundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_background");
	backgroundNode->setPosition(0, 0, 0);

	Ogre::Entity *grass = mSceneMgr->createEntity("boomb_grass", "boomb_grass.mesh");
	grass->setMaterialName("boomb_grass");
	grass->setCastShadows(true);
	Ogre::SceneNode *grassNode = backgroundNode->createChildSceneNode("boomb_grass");
	grassNode->setPosition(-300, 0, 1200);
	grassNode->setScale(2, 2, 2);
	grassNode->attachObject(grass);

	Ogre::Entity *tree = mSceneMgr->createEntity("boomb_tree", "boomb_tree.mesh");
	tree->setMaterialName("boomb_tree");
	tree->setCastShadows(true);
	Ogre::SceneNode *treeNode = backgroundNode->createChildSceneNode("boomb_tree");
	treeNode->setPosition(-100, 0, -100);
	treeNode->setScale(10, 10, 10);
	treeNode->attachObject(tree);

	Ogre::Entity *building = mSceneMgr->createEntity("boomb_building", "boomb_building.mesh");
	Ogre::SceneNode *buildingNode = backgroundNode->createChildSceneNode("boomb_building");
	building->setMaterialName("boomb_building");
	building->setCastShadows(true);
	buildingNode->setPosition(400, 0, -400);
	buildingNode->setScale(500,500,500);
	q1 = buildingNode->getOrientation();
	q2 = Ogre::Quaternion(Ogre::Degree(45), Ogre::Vector3::UNIT_Y);
	buildingNode->setOrientation(q1*q2);
	buildingNode->attachObject(building);


	// Set Fog

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("directlight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(Ogre::Vector3(-1, -1, -1));

	// Create Overlay

	this->createAnimation();
}

void MenuSceneClass::createAnimation(void)
{

	Ogre::SceneNode *titleNode = static_cast<Ogre::SceneNode *>(mSceneMgr->getRootSceneNode()->getChild("boomb_title"));
	Ogre::Animation *anim = mSceneMgr->createAnimation("title_animation", 3);
	anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	Ogre::NodeAnimationTrack *track = anim->createNodeTrack(0, titleNode);
	Ogre::TransformKeyFrame *key = track->createNodeKeyFrame(0);
	key->setTranslate(Ogre::Vector3(0,750,700));
	Ogre::Quaternion q1 = titleNode->getOrientation();
	Ogre::Quaternion q2(Ogre::Degree(45), Ogre::Vector3::UNIT_X);
	key->setRotation(q2);
	key = track->createNodeKeyFrame(1.5);
	key->setTranslate(Ogre::Vector3(0,750,720));
	key->setRotation(q2);
	key = track->createNodeKeyFrame(3);
	key->setTranslate(Ogre::Vector3(0,750,700));
	key->setRotation(q2);

	mTitleAnimState = mSceneMgr->createAnimationState("title_animation");
	mTitleAnimState->setEnabled(true);
	mTitleAnimState->setLoop(true);

	Ogre::SceneNode *backgroundNode = static_cast<Ogre::SceneNode *>(mSceneMgr->getRootSceneNode()->getChild("boomb_background"));
	anim = mSceneMgr->createAnimation("background_animation", 30);
	anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	track = anim->createNodeTrack(0, backgroundNode);
	key = track->createNodeKeyFrame(0);
	key->setTranslate(Ogre::Vector3(20,0,20));
	key = track->createNodeKeyFrame(15);
	key->setTranslate(Ogre::Vector3(-20,0,-20));
	key = track->createNodeKeyFrame(30);
	key->setTranslate(Ogre::Vector3(20,0,20));
	mBackgroundAnimState = mSceneMgr->createAnimationState("background_animation");
	mBackgroundAnimState->setEnabled(true);
	mBackgroundAnimState->setLoop(true);
}