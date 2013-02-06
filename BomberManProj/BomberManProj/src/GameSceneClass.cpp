#include "GameSceneClass.h"


GameSceneClass::GameSceneClass(void)
{
	nonNPCAnimState = NULL;
	NPCAnimState = NULL;
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

	Ogre::Entity *nonNPC = mSceneMgr->createEntity("nonnpcplayer","ogrehead.mesh");
	nonNPC->setCastShadows(true);
	nonNPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("nonnpcplayerNode");
	nonNPCPlayerNode->setPosition(0, 50, 0);
	nonNPCPlayerNode->attachObject(nonNPC);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("directlight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(Ogre::Vector3(-1, -1, -1));
}

void GameSceneClass::askingPlayer(int playerType, int eventType)
{
	switch(eventType)
	{
	case EVENT_UP:
		{
			if (playerType == NON_NPC){
				if (nonNPCAnimState){
					if (nonNPCAnimState->hasEnded()){
						movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), nonNPCAnimState);
					}
				} else {
					movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), nonNPCAnimState);
				}
			} else {
				if (NPCAnimState){
					if (NPCAnimState->hasEnded()){
						movingPlayer(NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), NPCAnimState);
					}
				} else {
					movingPlayer(NPC, NPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), NPCAnimState);
				}
			}
			break;
		}
	case EVENT_DOWN:
		{
			if (playerType == NON_NPC){
				if (nonNPCAnimState){
					if (nonNPCAnimState->hasEnded()){
						movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), nonNPCAnimState);
					}
				} else {
					movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), nonNPCAnimState);
				}
			} else {
				if (NPCAnimState){
					if (NPCAnimState->hasEnded()){
						movingPlayer(NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), NPCAnimState);
					}
				} else {
					movingPlayer(NPC,NPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), NPCAnimState);
				}
			}
			break;
		}
	case EVENT_LEFT:
		{
			if (playerType == NON_NPC){
				if (nonNPCAnimState){
					if (nonNPCAnimState->hasEnded()){
						movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), nonNPCAnimState);
					}
				} else {
					movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), nonNPCAnimState);
				}
			} else {
				if (NPCAnimState){
					if (NPCAnimState->hasEnded()){
						movingPlayer(NPC, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), NPCAnimState);
					}
				} else {
					movingPlayer(NPC,NPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), NPCAnimState);
				}
			}
			break;
		}
	case EVENT_RIGHT:
		{
			if (playerType == NON_NPC){
				if (nonNPCAnimState){
					if (nonNPCAnimState->hasEnded()){
						movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), nonNPCAnimState);
					}
				} else {
					movingPlayer(NPC,nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), nonNPCAnimState);
				}
			} else {
				if (NPCAnimState){
					if (NPCAnimState->hasEnded()){
						movingPlayer(NPC, nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), NPCAnimState);
					}
				} else {
					movingPlayer(NPC,NPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), NPCAnimState);
				}
			}
			break;
		}
	default:
		break;
	}
}

void GameSceneClass::movingPlayer(int playerType, Ogre::SceneNode *playerNode, Ogre::Vector3 direction,	Ogre::AnimationState *animState)
{
	Ogre::Animation *anim;
	if (playerType == NON_NPC){
		if (animState){
			mSceneMgr->destroyAnimation("nonnpcmoving");
		}
		anim = mSceneMgr->createAnimation("nonnpcmoving", DEFAULT_SPEED);
		nonNPCAnimState = mSceneMgr->createAnimationState("nonnpcmoving");
		animState = nonNPCAnimState;
	} else {
		if (animState){
			mSceneMgr->destroyAnimation("npcmoving");
		}
		anim = mSceneMgr->createAnimation("npcmoving", DEFAULT_SPEED);
		NPCAnimState = mSceneMgr->createAnimationState("npcmoving");
		animState = NPCAnimState;
	}
	anim->setInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::NodeAnimationTrack *track = anim->createNodeTrack(0, playerNode);
	Ogre::TransformKeyFrame *key = track->createNodeKeyFrame(0);
	Ogre::Vector3 currentPos = playerNode->getPosition();
	key->setTranslate(currentPos);
	key = track->createNodeKeyFrame(DEFAULT_SPEED);
	currentPos.x += direction.x;
	currentPos.y += direction.y;
	currentPos.z += direction.z;
	key->setTranslate(currentPos);

	animState->setEnabled(true);
	animState->setLoop(false);
}


/*
void GameSceneClass::animation()
{
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
*/