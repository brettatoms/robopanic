// Character.h: interface for the Character class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CHARACTER_H
#define _CHARACTER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "defines.h"
#include <ddraw.h>
#include "Sprite.h"

class SceneCoordinate;
class BoundingBox;

class Character: public Sprite{
public:
	Character(char *filename, int frames, int w, int h, int rows, int columns);
	virtual ~Character();

	void setCurrentAnimation(int animation);
	int getCurrentAnimation();
	void setAnimationSpeed(int speed);	
	void setCharacterMoving(bool b);
	bool getCharacterMoving();
	void move(Direction direction);	
	void moveDirection(Direction direction);
	Direction getCurrentDirection();
	void setCharacterState(State state);
	State getCharacterState();

	int getHealth();
	void changeHealth(int change);
	
	void setSpeed(int x, int y);
	void getSpeed(int &x, int &y);
	int getXSpeed(){ return speedX; };
	int getYSpeed(){ return speedY; };
	void shootBullet(LPDIRECTDRAWSURFACE surface);
	
	bool draw(LPDIRECTDRAWSURFACE surface);
	void animate();

	bool isCoordInside(int x, int y);
	void setPosition(int x, int y);
	SceneCoordinate *getPosition();
	SceneCoordinate *getArmPosition();
	void getPosition(int &x, int &y);
	SceneCoordinate *getCenterPoint();	
	BoundingBox *getBoundingBox();
	BoundingBox *getFeetBoundingBox();
protected:
	SceneCoordinate *sceneCoord;
	Direction characterDirection;
	Direction hitDirection;
	int positionX, positionY;
	int speedX, speedY;
	bool characterMoving;
	int health;
	State characterState;

	long lastMoveTime;
	long currentClockTime;
	long lastShootTime;
	long lastHitTime;
	
	int animationArray [4][4];
	int shootArray[4][2];
	int hitArray[4][2];
	int deadAnimationArray[4][2];
	int deadCounter;
	int currentFrame;
	int currentAnimation;
    int currentAnimationFrame;
	int currentShootFrame, currentHitFrame, currentDeadFrame;
	int animationSpeed;
	int animationCounter;
};

#endif // _CHARACTER_H
