//Character.cpp

#include "Character.h"
#include "SceneCoordinate.h"
#include "BoundingBox.h"
#include "files.h"

Character::Character(char *filename, int frames,int w,int h,int rows,int columns):
		   Sprite(filename, frames, w, h, rows, columns){
	loadSprites();
	positionX=positionY=speedX=speedY=0;
	currentFrame=currentAnimation=currentAnimationFrame=0;
	currentShootFrame=currentHitFrame=currentDeadFrame=0;
	deadCounter=0;
	characterMoving=false;
	characterState=ALIVE;
	animationSpeed=animationCounter=0;
	lastMoveTime=GetTickCount();
	lastHitTime=lastShootTime=0;
	//currentClockTime=GetTickCount();

	animationArray[SOUTH][0]=1;
	animationArray[SOUTH][1]=0;
	animationArray[SOUTH][2]=1;
	animationArray[SOUTH][3]=2;
	animationArray[NORTH][0]=10;
	animationArray[NORTH][1]=9;
	animationArray[NORTH][2]=10;
	animationArray[NORTH][3]=11;
	animationArray[EAST][0]=19;
	animationArray[EAST][1]=18;
	animationArray[EAST][2]=19;
	animationArray[EAST][3]=20;
	animationArray[WEST][0]=28;
	animationArray[WEST][1]=27;
	animationArray[WEST][2]=28;
	animationArray[WEST][3]=29;

	shootArray[SOUTH][0]=3;
	shootArray[SOUTH][1]=4;
	shootArray[NORTH][0]=12;
	shootArray[NORTH][1]=13;
	shootArray[EAST][0]=21;
	shootArray[EAST][1]=22;
	shootArray[WEST][0]=30;
	shootArray[WEST][1]=31;

	hitArray[SOUTH][0]=5;
	hitArray[SOUTH][1]=6;
	hitArray[NORTH][0]=14;
	hitArray[NORTH][1]=15;
	hitArray[EAST][0]=23;
	hitArray[EAST][1]=24;
	hitArray[WEST][0]=32;
	hitArray[WEST][1]=33;

	deadAnimationArray[SOUTH][0]=7;	
	deadAnimationArray[SOUTH][1]=8;	
	deadAnimationArray[NORTH][0]=16;	
	deadAnimationArray[NORTH][1]=17;	
	deadAnimationArray[EAST][0]=25;	
	deadAnimationArray[EAST][1]=26;	
	deadAnimationArray[WEST][0]=34;	
	deadAnimationArray[WEST][1]=35;		

	health = 100;
}
					 
////////////////////////////////////////////////////////

Character::~Character(){

}

////////////////////////////////////////////////////////

bool Character::isCoordInside(int x, int y){
	return TRUE;
}

////////////////////////////////////////////////////////

BoundingBox *Character::getBoundingBox(){
	return new BoundingBox(positionY, positionX, 
		positionY+spriteHeight, positionX+spriteWidth);

}

////////////////////////////////////////////////////////

// the feet bounding box is not as wide as the character and is only
// the bottom 1/8 of the character
BoundingBox *Character::getFeetBoundingBox(){	
	return new BoundingBox(positionY+(7*(spriteHeight>>3)), positionX+7, 
		positionY+spriteHeight, positionX+spriteWidth-7);

}

////////////////////////////////////////////////////////

void Character::changeHealth(int change){
	health+=change;
	if(health>100)
		health=100;
	if(health<0)
		health=0;
}

////////////////////////////////////////////////////////

void Character::setCharacterMoving(bool b){
	characterMoving=b;
}

////////////////////////////////////////////////////////

bool Character::getCharacterMoving(){
	return characterMoving;
}

////////////////////////////////////////////////////////

void Character::setAnimationSpeed(int speed){
	animationSpeed=speed;
}

////////////////////////////////////////////////////////

void Character::move(Direction direction){
	characterMoving=true;
	characterDirection = direction;
	switch(characterDirection){
	case NORTH:
		positionY-=speedY;
		break;
	case EAST:
		positionX+=speedX;
		break;
	case SOUTH:
		positionY+=speedY;
		break;
	case WEST:
		positionX-=speedX;
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////

void Character::moveDirection(Direction direction){
	switch(direction){
	case NORTH:
		positionY-=12;
		break;
	case EAST:
		positionX+=12;
		break;
	case SOUTH:
		positionY+=12;
		break;
	case WEST:
		positionX-=12;
		break;
	default:
		break;
	}
		/*
	case NORTH:
		positionY-=20;
		break;
	case EAST:
		positionX+=20;
		break;
	case SOUTH:
		positionY+=20;
		break;
	case WEST:
		positionX-=20;
		break;
	default:
		break;
	}
	*/
}

////////////////////////////////////////////////////////

void Character::setPosition(int x, int y){
	positionX=x;
	positionY=y;
}

////////////////////////////////////////////////////////

void Character::getPosition(int &x, int &y){
	x=positionX;
	y=positionY;
}

////////////////////////////////////////////////////////

int Character::getHealth(){
	return health;
}

////////////////////////////////////////////////////////

Direction Character::getCurrentDirection(){
	return characterDirection;
}

////////////////////////////////////////////////////////

int Character::getCurrentAnimation(){
	return currentAnimation;
}

////////////////////////////////////////////////////////

void Character::setCurrentAnimation(int animation){
	currentAnimation=animation;
}

////////////////////////////////////////////////////////

void Character::setSpeed(int x, int y){
	speedX=x;
	speedY=y;
}

////////////////////////////////////////////////////////

void Character::getSpeed(int &xSpeed, int &ySpeed){
	xSpeed = speedX;
	ySpeed = speedY;
}

////////////////////////////////////////////////////////

bool Character::draw(LPDIRECTDRAWSURFACE surface){
	if(characterState!=DEAD){
	if( !(this->Sprite::draw(surface, positionX, positionY, currentFrame)) )
		return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////

void Character::setCharacterState(State state){
	if(state == ALIVE)
		characterState=state;
	if(characterState != DYING && characterState != DEAD)
		characterState=state;
	if(state==HIT){
		lastHitTime=GetTickCount();
		if(health<=0){
			characterState=DYING;
		}
		//characterState==RECOVERING;
	}
}

////////////////////////////////////////////////////////

State Character::getCharacterState(){
	return characterState;
}

////////////////////////////////////////////////////////

SceneCoordinate *Character::getPosition(){
	return new SceneCoordinate(positionX, positionY);
}

////////////////////////////////////////////////////////

SceneCoordinate *Character::getArmPosition(){
	switch(characterDirection){
	case NORTH:
		return new SceneCoordinate(positionX-10, positionY+5);
		break;
	case EAST:
		return new SceneCoordinate(positionX+spriteWidth-30, positionY+19);
		break;
	case SOUTH:
		return new SceneCoordinate(positionX+spriteWidth-22, positionY+spriteHeight-25);
		break;
	case WEST:
		return new SceneCoordinate(positionX, positionY+22);
		break;
	default:
		return new SceneCoordinate(positionX, positionY);
		break;
	}
	
}


////////////////////////////////////////////////////////

SceneCoordinate *Character::getCenterPoint(){	
	return new SceneCoordinate(positionX+(spriteWidth>>1),positionY+(spriteHeight>>1));
}

////////////////////////////////////////////////////////

void Character::animate(){
	//if(characterState==ALIVE){
	if(characterMoving && characterState == ALIVE){
		if((GetTickCount()-lastMoveTime) > animationSpeed){
			animationCounter=0;
			lastMoveTime=GetTickCount();
			currentAnimationFrame++;

			if(currentAnimationFrame>3)
				currentAnimationFrame=0;

			currentFrame=animationArray[currentAnimation][currentAnimationFrame];
		}
	}

	else if(characterState == SHOOTING){
		if((GetTickCount()-lastShootTime) > animationSpeed){
			lastShootTime=GetTickCount();
			if(currentShootFrame == 2){
				currentShootFrame=0;
				characterState=ALIVE;
				currentFrame=animationArray[currentAnimation][0];
			}
			else{
				currentFrame=shootArray[currentAnimation][currentShootFrame];
				currentShootFrame++;
			}
			//qcurrentShootFrame++;
		}
	}

	else if(characterState == HIT){
		currentFrame=hitArray[currentAnimation][currentHitFrame];
		currentHitFrame++;
		if(currentHitFrame == 2){
			characterState=RECOVERING;
			currentHitFrame=0;
			currentAnimationFrame=0;
		}
	}

	else if(characterState == RECOVERING){
		if( (GetTickCount() - lastHitTime) > 1000)
			characterState=ALIVE;

		if(characterMoving){
			if((GetTickCount()-lastMoveTime) > animationSpeed){
				animationCounter=0;
				lastMoveTime=GetTickCount();
				currentAnimationFrame++;

				if(currentAnimationFrame>3)
					currentAnimationFrame=0;

				currentFrame=animationArray[currentAnimation][currentAnimationFrame];
			}
		}
	}

	else if(characterState == DYING){
		deadCounter++;
		if(deadCounter==10){
			currentDeadFrame++;
		}

		if(deadCounter==30){
			currentDeadFrame++;
		}

		if (currentDeadFrame==2){
			dprintf(("SET STATE TO DEAD\n"));
			characterState=DEAD;
		}
		else{
			currentFrame=deadAnimationArray[currentAnimation][currentDeadFrame];
		}
	}

	else if(characterState == DEAD){
		currentDeadFrame=0;
		deadCounter=0;
		dprintf(("GETTING INTO DEAD STATE\n"));
	}
	
	else{
		currentFrame=animationArray[currentAnimation][0];
	}
}