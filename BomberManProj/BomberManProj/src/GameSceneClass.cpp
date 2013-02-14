#include "GameSceneClass.h"


GameSceneClass::GameSceneClass(void)
{
	nonNPCAnimState = NULL;
	NPCAnimState = NULL;
	isSpaceKeyDown = false;
	bombIndex = 0;
}


GameSceneClass::~GameSceneClass(void)
{
}

void GameSceneClass::initGameData(void)
{
	nonNPCPlayer.playerType = NON_NPC;
	nonNPCPlayer.bombAvailable = 1;
	nonNPCPlayer.pos = map.nonNPCStartPos;
	nonNPCPlayer.power = 1;

	NPCPlayer.playerType = NPC;
	NPCPlayer.bombAvailable = 1;
	NPCPlayer.pos = map.NPCStartPos;
	NPCPlayer.power = 1;
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

	for (int x = 0; x < 15; ++x){
		for (int y = 0; y < 13; ++y){
			if (map.getMapAtPos(x, y) == MAP_DESTROYABLE)
			{
				Ogre::Entity *destroyableBlock = mSceneMgr->createEntity("boomb_destroyableBlock.mesh");
				destroyableBlock->setMaterialName("boomb_testDestroyable");
				destroyableBlock->setCastShadows(true);
				mapNode[x][y] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				mapNode[x][y]->setPosition(0, 0, 0);
				mapNode[x][y]->setPosition(getWorldCoord(Ogre::Vector2(x,y)));
				mapNode[x][y]->attachObject(destroyableBlock);
			} else if (map.getMapAtPos(x, y) == MAP_INDESTROYABLE){
				Ogre::Entity *indestroyableBlock = mSceneMgr->createEntity("boomb_indestroyableBlock.mesh");
				indestroyableBlock->setMaterialName("boomb_testInDestroyable");
				indestroyableBlock->setCastShadows(true);
				mapNode[x][y] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				mapNode[x][y]->setPosition(0, 0, 0);
				mapNode[x][y]->setPosition(getWorldCoord(Ogre::Vector2(x,y)));
				mapNode[x][y]->attachObject(indestroyableBlock);
			}
		}
	}

	Ogre::Entity *nonNPC = mSceneMgr->createEntity("nonnpcplayer","ogrehead.mesh");
	nonNPC->setCastShadows(true);
	nonNPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("nonnpcplayerNode");
	nonNPCPlayerNode->setPosition(getWorldCoord(nonNPCPlayer.pos));
	nonNPCPlayerNode->attachObject(nonNPC);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("pointlight");
	l->setType(Ogre::Light::LT_POINT);
	l->setPosition(Ogre::Vector3(0, 700, 0));
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setSpecularColour(0.5, 0.5, 0.5);
}

void GameSceneClass::askingPlayer(int playerType, int eventType)
{
	switch(eventType)
	{
	case EVENT_UP:
		{
			if (playerType == NON_NPC){
				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y - 1);
				if (mapType == MAP_NONE){
					if (nonNPCAnimState){
						if (nonNPCAnimState->hasEnded()){
							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
							nonNPCPlayer.destinationPos.y--;
							movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), nonNPCAnimState);
						}
					} else {
						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
						nonNPCPlayer.destinationPos.y--;
						movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), nonNPCAnimState);
					}
				} // else if
			} else {
				int mapType = map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y - 1);
				if (mapType == MAP_NONE){
					if (NPCAnimState){
						if (NPCAnimState->hasEnded()){
							NPCPlayer.destinationPos = NPCPlayer.pos;
							NPCPlayer.destinationPos.y--;
							movingPlayer(NPC, NPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), NPCAnimState);
						}
					} else {
						NPCPlayer.destinationPos = NPCPlayer.pos;
						NPCPlayer.destinationPos.y--;
						movingPlayer(NPC, NPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), NPCAnimState);
					}
				} // else if
			}
			break;
		}
	case EVENT_DOWN:
		{
			if (playerType == NON_NPC){
				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y + 1);
				if (mapType == MAP_NONE){
					if (nonNPCAnimState){
						if (nonNPCAnimState->hasEnded()){
							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
							nonNPCPlayer.destinationPos.y++;
							movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), nonNPCAnimState);
						}
					} else {
						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
						nonNPCPlayer.destinationPos.y++;
						movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), nonNPCAnimState);
					}
				} // else if
			} else {
				int mapType = map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y + 1);
				if (mapType == MAP_NONE){
					if (NPCAnimState){
						if (NPCAnimState->hasEnded()){
							NPCPlayer.destinationPos = NPCPlayer.pos;
							NPCPlayer.destinationPos.y++;
							movingPlayer(NPC, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), NPCAnimState);
						}
					} else {
						NPCPlayer.destinationPos = NPCPlayer.pos;
						NPCPlayer.destinationPos.y++;
						movingPlayer(NPC,NPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), NPCAnimState);
					}
				} // else if
			}
			break;
		}
	case EVENT_LEFT:
		{
			if (playerType == NON_NPC){
				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x - 1, nonNPCPlayer.pos.y);
				if (mapType == MAP_NONE){
					if (nonNPCAnimState){
						if (nonNPCAnimState->hasEnded()){
							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
							nonNPCPlayer.destinationPos.x--;
							movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), nonNPCAnimState);
						}
					} else {
						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
						nonNPCPlayer.destinationPos.x--;
						movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), nonNPCAnimState);
					}
				} // else if
			} else {
				int mapType = map.getMapAtPos(NPCPlayer.pos.x - 1, NPCPlayer.pos.y);
				if (mapType == MAP_NONE){
					if (NPCAnimState){
						if (NPCAnimState->hasEnded()){
							NPCPlayer.destinationPos = NPCPlayer.pos;
							NPCPlayer.destinationPos.x--;
							movingPlayer(NPC, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), NPCAnimState);
						}
					} else {
						NPCPlayer.destinationPos = NPCPlayer.pos;
						NPCPlayer.destinationPos.x--;
						movingPlayer(NPC,NPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), NPCAnimState);
					}
				} // else if
			}
			break;
		}
	case EVENT_RIGHT:
		{
			if (playerType == NON_NPC){
				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x + 1, nonNPCPlayer.pos.y);
				if (mapType == MAP_NONE){
					if (nonNPCAnimState){
						if (nonNPCAnimState->hasEnded()){
							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
							nonNPCPlayer.destinationPos.x++;
							movingPlayer(NON_NPC, nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), nonNPCAnimState);
						}
					} else {
						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
						nonNPCPlayer.destinationPos.x++;
						movingPlayer(NON_NPC,nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), nonNPCAnimState);
					}
				} // else if
			} else {
				int mapType = map.getMapAtPos(NPCPlayer.pos.x + 1, NPCPlayer.pos.y);
				if (mapType == MAP_NONE){
					if (NPCAnimState){
						if (NPCAnimState->hasEnded()){
							NPCPlayer.destinationPos = NPCPlayer.pos;
							NPCPlayer.destinationPos.x++;
							movingPlayer(NPC, nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), NPCAnimState);
						}
					} else {
						NPCPlayer.destinationPos = NPCPlayer.pos;
						NPCPlayer.destinationPos.x++;
						movingPlayer(NPC,NPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), NPCAnimState);
					}
				} // else if
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
	currentPos += direction;
	key->setTranslate(currentPos);

	animState->setEnabled(true);
	animState->setLoop(false);
}

void GameSceneClass::updatePlayerPos(void)
{
	if (nonNPCAnimState->getTimePosition() >= DEFAULT_SPEED/2){
		nonNPCPlayer.pos = nonNPCPlayer.destinationPos;
	}
	/*
	if (NPCAnimState->getTimePosition() >= DEFAULT_SPEED/2){
		NPCPlayer.pos = NPCPlayer.destinationPos;
	}
	*/
}

Ogre::Vector3 GameSceneClass::getWorldCoord(Ogre::Vector2 pos)
{
	Ogre::Vector3 rtn;
	rtn.y = 0;
	rtn.x = GRID_SIZE*(pos.x - 7);
	rtn.z = GRID_SIZE*(pos.y - 6);
	return rtn;
}

int GameSceneClass::thromBomb(PlayerClass player)
{
	BombClass tempBomb;
	tempBomb.bombType = NON_NPC_BOMB;
	tempBomb.countDown = 3.0;
	tempBomb.pos = nonNPCPlayer.pos;
	tempBomb.power = nonNPCPlayer.power;
	tempBomb.node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	tempBomb.node->setPosition(getWorldCoord(tempBomb.pos));
	Ogre::Entity *bombEntity = mSceneMgr->createEntity("boomb_bomb.mesh");
	bombEntity->setCastShadows(true);
	tempBomb.node->attachObject(bombEntity);
	bombPool.insert(std::pair<int, BombClass>(bombIndex++, tempBomb));
	map.setMapAtPos(tempBomb.pos.x, tempBomb.pos.y, MAP_BOMB);
	nonNPCPlayer.bombAvailable--;
	return (bombIndex-1);
}

void GameSceneClass::calculateBombArea(BombClass &bomb)
{
	bomb.bombType;
}

void GameSceneClass::updateBombInfo(const Ogre::FrameEvent& evt)
{
	for (std::map<int, BombClass>::iterator iter = bombPool.begin(); iter != bombPool.end();){
		iter->second.countDown-=evt.timeSinceLastFrame;
		if (iter->second.countDown <= 0){
			mSceneMgr->destroySceneNode(iter->second.node);
			map.setMapAtPos(iter->second.pos.x, iter->second.pos.y, MAP_NONE);
			calculateBombArea(iter->second);
			if (iter->second.bombType == NON_NPC_BOMB){
				nonNPCPlayer.bombAvailable++;
			} else {
				NPCPlayer.bombAvailable++;
			}
			bombPool.erase(iter++);
		} else {
			++iter;
		}
	}
}