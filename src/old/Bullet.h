// Bullet.h: interface for the Bullet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BULLET_H__9C9F07E1_9C24_11D3_8A6E_00902793B09B__INCLUDED_)
#define AFX_BULLET_H__9C9F07E1_9C24_11D3_8A6E_00902793B09B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <list>
#include "ddraw.h"
#include "defines.h"
#include "Sprite.h"

class Sprite;
class SceneCoordinate;
class BoundingBox;

typedef enum {CharBullet=0,EnemyBullet, NUM_BULLET_TYPES} BulletType;

using namespace std;  // for STL

class Bullet  
{
public:
	Bullet(char *filename, int frames, int w, int h, int rows, int columns, BulletType type);
	virtual ~Bullet();
	void setSpeed(int dx, int dy);
	void setPosition(SceneCoordinate *point);
	void setBulletState(State state);
	State getBulletState(); 
	bool draw(LPDIRECTDRAWSURFACE surface);
	void setDirection(Direction direction);
	void move();
	BoundingBox* getBoundingBox();
private:
	static Sprite *bullet[NUM_BULLET_TYPES];
	int positionX, positionY;
	int speedX, speedY; 
	Direction direction;
	State bulletState;
	BulletType type;
};

typedef list<Bullet *> BulletList;


#endif // !defined(AFX_BULLET_H__9C9F07E1_9C24_11D3_8A6E_00902793B09B__INCLUDED_)
