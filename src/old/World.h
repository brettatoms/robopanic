#ifndef _WORLD_H
#define _WORLD_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "defines.h"
#include <ddraw.h>
#include "entity.h"
#include "Bullet.h"


class Scene;
class Character;
class Bullet;
class Enemy;
class Entity;

class World{
public:
	World();
	~World(); 
	bool initialize();
	bool FC draw(LPDDSurface surface);
	bool changeScene(Direction d);
	bool restore();
	void release();
	bool FC scrollInNewScene(LPDDSurface surface);
	//elist[0] is the entity on top
	int getEntitiesAt(int x, int y, EntityList *elist); 	
	Character *character;		
	Scene *currScene;
	bool FC composeFrame();
	void advanceCharacter();
	void advanceEnemies();
	void advanceBullets();
	void checkCharacterCollision();
	Direction getCharacterDirection();
	void setCharacterDirection(Direction direction);
	bool createBullet();
	bool createEnemyBullet(Enemy *enemy);	
	void reactToAction(int actionType);
	bool hasSceneChanged(){ return sceneHasChanged; }
	bool approveEntityMove(Entity *entity, int newX, int newY);
	bool checkLevelComplete();
	Scene *prevScene;
private:			
	void pushActionCallback();
	void pickupActionCallback();
	bool sceneHasChanged;
	Direction characterDirection;
	Direction prevDirection;
	BulletList bulletList, enemyBulletList;
	long lastBulletTime;
	int bulletCounter;
};

#endif  /* _WORLD_H */
