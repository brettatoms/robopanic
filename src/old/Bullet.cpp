// Bullet.cpp: implementation of the Bullet class.
//
//////////////////////////////////////////////////////////////////////

//#include ""
#include "Bullet.h"
#include "SceneCoordinate.h"
#include "BoundingBox.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Sprite* Bullet::bullet[]={NULL,NULL};

Bullet::Bullet(char *filename, int frames, int w, int h, int rows, int columns, BulletType type)
{
	this->type=type;
	bulletState=ALIVE;
	if(bullet[type]==NULL){
		bullet[type]=new Sprite(filename,frames,w,h,rows,columns);
		bullet[type]->loadSprites();
	}

	if(bullet[type]->surfaceLost() == true){
		bullet[type]->restore();
		bullet[type]->load(filename);
		bullet[type]->loadSprites();
	}
}

Bullet::~Bullet()
{

}


void Bullet::setSpeed(int dx, int dy){
	speedX=dx;
	speedY=dy;
}

void Bullet::setDirection(Direction direction){
	this->direction=direction;
}

void Bullet::setBulletState(State state){
	bulletState=state;
}

State Bullet::getBulletState(){
	return bulletState;
}

void Bullet::move(){
	switch(direction){
	case NORTH:	
		positionY-=speedY;
		if(positionY<0)
			bulletState=DEAD;
		break;
	case EAST:
		positionX+=speedX;	
		if(positionX>640)
			bulletState=DEAD;
		break;
	case SOUTH:	
		positionY+=speedY;
		if(positionY+bullet[type]->height()>480)
			bulletState=DEAD;
		break;
	case WEST:
		positionX-=speedX;
		if(positionX<0)
			bulletState=DEAD;
		break;
	}
}

void Bullet::setPosition(SceneCoordinate *point){
	positionX=point->x;
	positionY=point->y;
}

BoundingBox* Bullet::getBoundingBox(){
	//if(type == CharBullet){
	return new BoundingBox(positionY+17, positionX+12,positionY+bullet[type]->height()-17,
					   positionX+bullet[type]->width()-17);
	//}
	/*else{
		return new BoundingBox(positionY, positionX,positionY+bullet[type]->height(),
						   positionX+bullet[type]->width());
	}*/
}

bool Bullet::draw(LPDIRECTDRAWSURFACE surface){
	//if(type == CharBullet){
	return bullet[type]->draw(surface, positionX, positionY, 0);
//	}
	/*else{
		if(direction == NORTH || direction == SOUTH){
			return bullet[type]->draw(surface, positionX, positionY, 0);
		}
		else if (direction == EAST || direction == WEST){
			return bullet[type]->draw(surface, positionX, positionY, 1);
		}
		else return false;
	}
	return true;*/
}
