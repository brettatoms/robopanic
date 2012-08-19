// Enemy.h: interface for the Enemy class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ENEMY_H_
#define _ENEMY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "ddraw.h"
#include "defines.h"
#include "SceneCoordinate.h"
//#include "SceneCoordinate.h"


using namespace std; // for STL

typedef enum {ENEMY1=0, ENEMY2, NUM_ENEMY_TYPES} EnemyType;
typedef enum {RANDOM, TRACKING, SHOOT, ATTACK, BEEN_HIT, RETREAT} AIState;
class BoundingBox;
class Sprite;


class Enemy  {
public:
	Enemy(EnemyType type);
	virtual ~Enemy();
	bool init();
	void setSpeed(int speed);
	int getSpeed();
	void setPosition(int x, int y);
	void setAnimationSpeed(int speed);
	void move();
	void moveBack(Direction direction);
	bool draw(LPDIRECTDRAWSURFACE surface);
	bool animate();
	void giveCharacterCoords(int x, int y);
	void checkLineOfSight();
	void checkOnScreen();
	void setCurrentAnimation(Direction direct);
	void changeHealth(int change);
	int getWidth();
	int getHeight();
	State getEnemyState();
	void setAIState(AIState state);
	AIState getAIState();
	SceneCoordinate* getArmPosition();
	BoundingBox* getBoundingBox();
	BoundingBox* getFeetBoundingBox();
	SceneCoordinate* getCenterPoint();
	Direction getCurrentDirection();
	static Sprite *spriteArray[NUM_ENEMY_TYPES];
	bool restore();
	void release();

private:
	int positionX, positionY;
	int charX, charY;
	EnemyType type;
	State enemyState;
	AIState aiState, prevAiState;

	int speedX, speedY;
	int health;

	int currentFrame;
	int currentShootFrame;
	int currentHitFrame;
	int currentAnimation;
	int currentDeadFrame;
    int currentAnimationFrame;

	int animationSpeed;
	int animationCounter;
	long lastMoveTime;
	long lastShootTime;
	long lastHitTime;
	long lastDeadTime;
	long currentClockTime;

	int currentDirectionCounter;
	int stateCounter;

	Direction direction;

	int shootArray[4][2];
	int hitArray[4][2];
	int deadAnimationArray[4][2];
	int animationArray [4][4];
};

typedef list<Enemy *> EnemyList;

#endif // _ENEMY_H_
