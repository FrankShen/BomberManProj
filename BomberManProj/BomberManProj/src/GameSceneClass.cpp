#include "GameSceneClass.h"


GameSceneClass::GameSceneClass(void)
{
	animState = NULL;
}


GameSceneClass::~GameSceneClass(void)
{
}

void GameSceneClass::createSceneMgr(Ogre::Root *_root)
{
	mSceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "MenuSceneMgr");

}

void GameSceneClass::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("GameCamera");

	mCamera->setPosition(Ogre::Vector3(0,800,1000));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
}


void GameSceneClass::createScene(void)
{
	Ogre::Entity* tempBoard = mSceneMgr->createEntity("boomb_tempboard", "boomb_tempboard.mesh");
	tempBoard->setMaterialName("boomb_tempboard");
	Ogre::SceneNode* tempBoardNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_tempboard");
	tempBoardNode->setScale(7.0, 7.0, 7.0);
	tempBoardNode->setPosition(0, 0, 0);
	Ogre::Quaternion q1 = tempBoardNode->getOrientation();
	Ogre::Quaternion q2(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
	tempBoardNode->setOrientation(q1*q2);
	tempBoardNode->attachObject(tempBoard);

	Ogre::Entity *test = mSceneMgr->createEntity("test","ogrehead.mesh");
	test->setCastShadows(true);
	testNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("testnode");
	testNode->setPosition(0, 50, 0);
	testNode->attachObject(test);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("directlight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(Ogre::Vector3(-1, -1, -1));
}

void GameSceneClass::animation()
{
	if (animState)
	{
		mSceneMgr->destroyAnimation("moving");
	}
	Ogre::Animation *anim = mSceneMgr->createAnimation("moving", 1);
	anim->setInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::NodeAnimationTrack *track = anim->createNodeTrack(0, testNode);
	Ogre::TransformKeyFrame *key = track->createNodeKeyFrame(0);
	Ogre::Vector3 currentPos = testNode->getPosition();
	key->setTranslate(currentPos);
	key = track->createNodeKeyFrame(1);
	currentPos.z += 70;
	key->setTranslate(currentPos);

	animState = mSceneMgr->createAnimationState("moving");
	animState->setEnabled(true);
	animState->setLoop(false);
}