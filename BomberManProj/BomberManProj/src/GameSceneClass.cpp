#include "GameSceneClass.h"


GameSceneClass::GameSceneClass(void)
{
	nonNPCAnimState = NULL;
	NPCAnimState = NULL;
	isSpaceKeyDown = false;
	bombIndex = 0;
	audiere::AudioDevicePtr device(audiere::OpenDevice());
	explodeSound = audiere::OpenSound(device, "explode.mp3", false);
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
	nonNPCPlayer.health = 3;
	nonNPCPlayer.invincible = 0.0f;
	nonNPCPlayer.speed = 0.3f;

	NPCPlayer.playerType = NPC;
	NPCPlayer.bombAvailable = 1;
	NPCPlayer.pos = map.NPCStartPos;
	NPCPlayer.power = 1;
	NPCPlayer.health = 3;
	NPCPlayer.invincible = 0.0f;
	NPCPlayer.speed = 0.3f;
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

	Ogre::Entity *npc = mSceneMgr->createEntity("npcplayer", "ogrehead.mesh");
	npc->setCastShadows(true);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("npcplayerNode");
	NPCPlayerNode->setPosition(getWorldCoord(NPCPlayer.pos));
	NPCPlayerNode->attachObject(npc);

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
				if (mapType  <= 2){
					if (nonNPCAnimState){
						if (nonNPCAnimState->hasEnded()){
							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
							nonNPCPlayer.destinationPos.y--;
							movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), nonNPCAnimState);
						}
					} else {
						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
						nonNPCPlayer.destinationPos.y--;
						movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), nonNPCAnimState);
					}
				} // else if
			} else {
				int mapType = map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y - 1);
				if (mapType  <= 2){
					if (NPCAnimState){
						if (NPCAnimState->hasEnded()){
							NPCPlayer.destinationPos = NPCPlayer.pos;
							NPCPlayer.destinationPos.y--;
							movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), NPCAnimState);
						}
					} else {
						NPCPlayer.destinationPos = NPCPlayer.pos;
						NPCPlayer.destinationPos.y--;
						movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), NPCAnimState);
					}
				} // else if
			}
			break;
		}
	case EVENT_DOWN:
		{
			if (playerType == NON_NPC){
				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y + 1);
				if (mapType  <= 2){
					if (nonNPCAnimState){
						if (nonNPCAnimState->hasEnded()){
							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
							nonNPCPlayer.destinationPos.y++;
							movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), nonNPCAnimState);
						}
					} else {
						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
						nonNPCPlayer.destinationPos.y++;
						movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), nonNPCAnimState);
					}
				} // else if
			} else {
				int mapType = map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y + 1);
				if (mapType  <= 2){
					if (NPCAnimState){
						if (NPCAnimState->hasEnded()){
							NPCPlayer.destinationPos = NPCPlayer.pos;
							NPCPlayer.destinationPos.y++;
							movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), NPCAnimState);
						}
					} else {
						NPCPlayer.destinationPos = NPCPlayer.pos;
						NPCPlayer.destinationPos.y++;
						movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), NPCAnimState);
					}
				} // else if
			}
			break;
		}
	case EVENT_LEFT:
		{
			if (playerType == NON_NPC){
				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x - 1, nonNPCPlayer.pos.y);
				if (mapType  <= 2){
					if (nonNPCAnimState){
						if (nonNPCAnimState->hasEnded()){
							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
							nonNPCPlayer.destinationPos.x--;
							movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), nonNPCAnimState);
						}
					} else {
						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
						nonNPCPlayer.destinationPos.x--;
						movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), nonNPCAnimState);
					}
				} // else if
			} else {
				int mapType = map.getMapAtPos(NPCPlayer.pos.x - 1, NPCPlayer.pos.y);
				if (mapType  <= 2){
					if (NPCAnimState){
						if (NPCAnimState->hasEnded()){
							NPCPlayer.destinationPos = NPCPlayer.pos;
							NPCPlayer.destinationPos.x--;
							movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), NPCAnimState);
						}
					} else {
						NPCPlayer.destinationPos = NPCPlayer.pos;
						NPCPlayer.destinationPos.x--;
						movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), NPCAnimState);
					}
				} // else if
			}
			break;
		}
	case EVENT_RIGHT:
		{
			if (playerType == NON_NPC){
				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x + 1, nonNPCPlayer.pos.y);
				if (mapType  <= 2){
					if (nonNPCAnimState){
						if (nonNPCAnimState->hasEnded()){
							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
							nonNPCPlayer.destinationPos.x++;
							movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), nonNPCAnimState);
						}
					} else {
						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
						nonNPCPlayer.destinationPos.x++;
						movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), nonNPCAnimState);
					}
				} // else if
			} else {
				int mapType = map.getMapAtPos(NPCPlayer.pos.x + 1, NPCPlayer.pos.y);
				if (mapType  <= 2){
					if (NPCAnimState){
						if (NPCAnimState->hasEnded()){
							NPCPlayer.destinationPos = NPCPlayer.pos;
							NPCPlayer.destinationPos.x++;
							movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), NPCAnimState);
						}
					} else {
						NPCPlayer.destinationPos = NPCPlayer.pos;
						NPCPlayer.destinationPos.x++;
						movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), NPCAnimState);
					}
				} // else if
			}
			break;
		}
	default:
		break;
	}
}

void GameSceneClass::movingPlayer(int playerType, float speed, Ogre::SceneNode *playerNode, Ogre::Vector3 direction,	Ogre::AnimationState *animState)
{
	Ogre::Animation *anim;
	if (playerType == NON_NPC){
		if (animState){
			mSceneMgr->destroyAnimation("nonnpcmoving");
		}
		anim = mSceneMgr->createAnimation("nonnpcmoving", speed);
		nonNPCAnimState = mSceneMgr->createAnimationState("nonnpcmoving");
		animState = nonNPCAnimState;
	} else {
		if (animState){
			mSceneMgr->destroyAnimation("npcmoving");
		}
		anim = mSceneMgr->createAnimation("npcmoving", speed);
		NPCAnimState = mSceneMgr->createAnimationState("npcmoving");
		animState = NPCAnimState;
	}
	anim->setInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::NodeAnimationTrack *track = anim->createNodeTrack(0, playerNode);
	Ogre::TransformKeyFrame *key = track->createNodeKeyFrame(0);
	Ogre::Vector3 currentPos = playerNode->getPosition();
	key->setTranslate(currentPos);
	key = track->createNodeKeyFrame(speed);
	currentPos += direction;
	key->setTranslate(currentPos);

	animState->setEnabled(true);
	animState->setLoop(false);
}

void GameSceneClass::updatePlayerPos(void)
{
	if (nonNPCAnimState->getTimePosition() >= nonNPCPlayer.speed/2){
		if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) != MAP_BOMB)
			map.setMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y, MAP_NONE);
		nonNPCPlayer.pos = nonNPCPlayer.destinationPos;
		if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) < 0){
			if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) == MAP_ADD_BOMB){
				nonNPCPlayer.bombAvailable++;
			} else if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) == MAP_ADD_POWER){
				nonNPCPlayer.power++;
			} else if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) == MAP_ADD_SPEED){
				nonNPCPlayer.speed *= 0.6;
			} else if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) == MAP_ADD_HEALTH){
				if (nonNPCPlayer.health < 3)
					nonNPCPlayer.health++;
			}
			int x = nonNPCPlayer.pos.x;
			int y = nonNPCPlayer.pos.y;
			//mSceneMgr->destroySceneNode(mapNode[x][y]);
		}
		map.setMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y, MAP_NON_NPC);
	}
}

void GameSceneClass::updateNPCPlayerPos(void)
{
	if (NPCAnimState->getTimePosition() >= NPCPlayer.speed/2){
		if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) != MAP_BOMB)
			map.setMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y, MAP_NONE);
		NPCPlayer.pos = NPCPlayer.destinationPos;
		if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) < 0){
			if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) == MAP_ADD_BOMB){
				NPCPlayer.bombAvailable++;
			} else if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) == MAP_ADD_POWER){
				NPCPlayer.power++;
			} else if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) == MAP_ADD_SPEED){
				NPCPlayer.speed *= 0.6;
			} else if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) == MAP_ADD_HEALTH){
				if (NPCPlayer.health < 3)
					NPCPlayer.health++;
			}
			int x = NPCPlayer.pos.x;
			int y = NPCPlayer.pos.y;
			//mSceneMgr->destroySceneNode(mapNode[x][y]);
		}
		map.setMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y, MAP_NPC);
	}
}

Ogre::Vector3 GameSceneClass::getWorldCoord(Ogre::Vector2 pos)
{
	Ogre::Vector3 rtn;
	rtn.y = 0;
	rtn.x = GRID_SIZE*(pos.x - 7);
	rtn.z = GRID_SIZE*(pos.y - 6);
	return rtn;
}

int GameSceneClass::thromBomb(PlayerClass &player)
{
	BombClass tempBomb;
	if (player.playerType == NON_NPC)
		tempBomb.bombType = NON_NPC_BOMB;
	else
		tempBomb.bombType = NPC_BOME;
	tempBomb.countDown = 3.0;
	tempBomb.pos = player.pos;
	tempBomb.power = player.power;
	tempBomb.node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	tempBomb.node->setPosition(getWorldCoord(tempBomb.pos));
	Ogre::Entity *bombEntity = mSceneMgr->createEntity("boomb_bomb.mesh");
	bombEntity->setCastShadows(true);
	tempBomb.node->attachObject(bombEntity);
	int x = tempBomb.pos.x;
	int y = tempBomb.pos.y;
	tempBomb.bombIdx = bombIndex;
	mapNode[x][y] = tempBomb.node;
	bombPool.insert(std::pair<int, BombClass>(bombIndex++, tempBomb));
	map.setMapAtPos(tempBomb.pos.x, tempBomb.pos.y, MAP_BOMB);
	player.bombAvailable--;
	return (bombIndex-1);
}

void GameSceneClass::explodeBomb(int bombIdx)
{
	explodeSound->play();
	std::map<int, BombClass>::iterator iter = bombPool.find(bombIdx);

	mSceneMgr->destroySceneNode(iter->second.node);
	map.setMapAtPos(iter->second.pos.x, iter->second.pos.y, MAP_NONE);

	calculateBombArea(iter->second);

	if (iter->second.bombType == NON_NPC_BOMB){
		nonNPCPlayer.bombAvailable++;
	} else {
		NPCPlayer.bombAvailable++;
	}
	bombPool.erase(iter);
}

void GameSceneClass::calculateBombArea(BombClass &bomb)
{
	// middle
	if (nonNPCPlayer.pos == bomb.pos){
		if (nonNPCPlayer.invincible <= 0){
			nonNPCPlayer.invincible = 3.0f;
			nonNPCPlayer.health--;
		}
	}
	if (NPCPlayer.pos == bomb.pos){
		if (NPCPlayer.invincible <= 0){
			NPCPlayer.invincible = 3.0f;
			NPCPlayer.health--;
		}
	}
	// DOWN
	for (int i = 1; i <= bomb.power; ++i){
		if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_INDESTROYABLE){
			break;
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_DESTROYABLE){
			int x = bomb.pos.x;
			int y = bomb.pos.y+i;
			mSceneMgr->destroySceneNode(mapNode[x][y]);
			map.setMapAtPos(bomb.pos.x, bomb.pos.y+i, MAP_NONE);
			addBonus(Ogre::Vector2(x,y));
			break;
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_NON_NPC){
			if (nonNPCPlayer.invincible <= 0){
				nonNPCPlayer.invincible = 3.0f;
				nonNPCPlayer.health--;
			}
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_NPC){
			if (NPCPlayer.invincible <= 0){
				NPCPlayer.invincible = 3.0f;
				NPCPlayer.health--;
			}
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_BOMB){
			for (std::map<int, BombClass>::iterator iter = bombPool.begin(); iter != bombPool.end(); ++iter){
				if(iter->second.pos.x == bomb.pos.x && iter->second.pos.y == bomb.pos.y+i){
					explodeBomb(iter->first);
					break;
				}
			}
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) < 0){
			int x = bomb.pos.x;
			int y = bomb.pos.y+i;
			mSceneMgr->destroySceneNode(mapNode[x][y]);
			map.setMapAtPos(x, y, MAP_NONE);
		}
	}
	// UP
	for (int i = 1; i <= bomb.power; ++i){
		if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_INDESTROYABLE){
			break;
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_DESTROYABLE){
			int x = bomb.pos.x;
			int y = bomb.pos.y-i;
			mSceneMgr->destroySceneNode(mapNode[x][y]);
			map.setMapAtPos(bomb.pos.x, bomb.pos.y-i, MAP_NONE);
			addBonus(Ogre::Vector2(x,y));
			break;
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_NON_NPC){
			if (nonNPCPlayer.invincible <= 0){
				nonNPCPlayer.invincible = 3.0f;
				nonNPCPlayer.health--;
			}
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_NPC){
			if (NPCPlayer.invincible <= 0){
				NPCPlayer.invincible = 3.0f;
				NPCPlayer.health--;
			}
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_BOMB){
			for (std::map<int, BombClass>::iterator iter = bombPool.begin(); iter != bombPool.end(); ++iter){
				if(iter->second.pos.x == bomb.pos.x && iter->second.pos.y == bomb.pos.y-i){
					explodeBomb(iter->first);
					break;
				}
			}
		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) < 0){
			int x = bomb.pos.x;
			int y = bomb.pos.y-i;

			mSceneMgr->destroySceneNode(mapNode[x][y]);
			map.setMapAtPos(x, y, MAP_NONE);
		}
	}
	// RIGHT
	for (int i = 1; i <= bomb.power; ++i){
		if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_INDESTROYABLE){
			break;
		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_DESTROYABLE){
			int x = bomb.pos.x+i;
			int y = bomb.pos.y;
			mSceneMgr->destroySceneNode(mapNode[x][y]);
			map.setMapAtPos(bomb.pos.x+i, bomb.pos.y, MAP_NONE);
			addBonus(Ogre::Vector2(x,y));
			break;
		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_NON_NPC){
			if (nonNPCPlayer.invincible <= 0){
				nonNPCPlayer.invincible = 3.0f;
				nonNPCPlayer.health--;
			}
		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_NPC){
			if (NPCPlayer.invincible <= 0){
				NPCPlayer.invincible = 3.0f;
				NPCPlayer.health--;
			}
		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_BOMB){
			for (std::map<int, BombClass>::iterator iter = bombPool.begin(); iter != bombPool.end(); ++iter){
				if(iter->second.pos.x == bomb.pos.x+i && iter->second.pos.y == bomb.pos.y){
					explodeBomb(iter->first);
					break;
				}
			}
		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) < 0){
			int x = bomb.pos.x+i;
			int y = bomb.pos.y;
			mSceneMgr->destroySceneNode(mapNode[x][y]);
			map.setMapAtPos(x, y, MAP_NONE);
		}
	}
	// LEFT
	for (int i = 1; i <= bomb.power; ++i){
		if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_INDESTROYABLE){
			break;
		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_DESTROYABLE){
			int x = bomb.pos.x-i;
			int y = bomb.pos.y;
			mSceneMgr->destroySceneNode(mapNode[x][y]);
			map.setMapAtPos(bomb.pos.x-i, bomb.pos.y, MAP_NONE);
			addBonus(Ogre::Vector2(x,y));
			break;
		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_NON_NPC){
			if (nonNPCPlayer.invincible <= 0){
				nonNPCPlayer.invincible = 3.0f;
				nonNPCPlayer.health--;
			}
		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_NPC){
			if (NPCPlayer.invincible <= 0){
				NPCPlayer.invincible = 3.0f;
				NPCPlayer.health--;
			}
		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_BOMB){
			for (std::map<int, BombClass>::iterator iter = bombPool.begin(); iter != bombPool.end(); ++iter){
				if(iter->second.pos.x == bomb.pos.x-i && iter->second.pos.y == bomb.pos.y){
					explodeBomb(iter->first);
					break;
				}
			}
		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) < 0){
			int x = bomb.pos.x-i;
			int y = bomb.pos.y;
			mSceneMgr->destroySceneNode(mapNode[x][y]);
			map.setMapAtPos(x, y, MAP_NONE);
		}
	}
}

void GameSceneClass::updateBombInfo(const Ogre::FrameEvent& evt)
{
	for (std::map<int, BombClass>::iterator iter = bombPool.begin(); iter != bombPool.end();){
		iter->second.countDown-=evt.timeSinceLastFrame;
		if (iter->second.countDown <= 0){
			std::map<int, BombClass>::iterator tempIter = iter;
			explodeBomb(tempIter->first);
			break;
		} else {
			iter++;
		}
	}
}

void GameSceneClass::updatePlayerInfo(const Ogre::FrameEvent& evt)
{
	if (NPCPlayer.invincible > 0){
		NPCPlayer.invincible -= evt.timeSinceLastFrame;
	}
	if (NPCPlayer.health <= 0){
		//.....
		// loser!
	}
	if (nonNPCPlayer.invincible > 0){
		nonNPCPlayer.invincible -= evt.timeSinceLastFrame;
	}
	if (nonNPCPlayer.health <= 0){
		//....
		// loser!
	}
}

void GameSceneClass::addBonus(Ogre::Vector2 pos)
{
	int x = pos.x;
	int y = pos.y;
	int randResult = rand()%11;
	if (randResult <= 1){
		map.setMapAtPos(x, y, MAP_ADD_BOMB);
		//..
	} else if (randResult <= 3){
		map.setMapAtPos(x, y, MAP_ADD_POWER);
		//..
	} else if (randResult <= 5){
		map.setMapAtPos(x, y, MAP_ADD_SPEED);
		//..
	} else if (randResult <= 6){
		map.setMapAtPos(x, y, MAP_ADD_HEALTH);
		//..
	}
}