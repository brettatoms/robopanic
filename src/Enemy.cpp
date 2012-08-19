// Enemy.cpp: implementation of the Enemy class.
//
//////////////////////////////////////////////////////////////////////

#include "Enemy.h"
#include "Sprite.h"
#include "files.h"
#include "BoundingBox.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Sprite* Enemy::spriteArray[]={NULL,NULL};

Enemy::Enemy(EnemyType type){
	positionX=positionY=0;
	this->type = type;

	currentFrame=currentAnimation=currentAnimationFrame=currentShootFrame=currentHitFrame=currentDeadFrame=0;
	animationSpeed=animationCounter=stateCounter=lastShootTime=lastHitTime=lastDeadTime=0;
	lastMoveTime=GetTickCount();
	direction=NORTH;
	enemyState=ALIVE;
	
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

	health = 50;
	aiState=prevAiState=RANDOM;
	currentDirectionCounter=0;
}

////////////////////////////////////////////////////////

Enemy::~Enemy(){

}

////////////////////////////////////////////////////////

// called once to initialize the Enemy
bool Enemy::init(){
	//dprintf(("Entering Enemy::Init\n"));
	//positionX=positionY=100;
	srand(GetTickCount());

	if(spriteArray[type]==NULL){
		switch(type){
		case ENEMY1:
			dprintf(("About to create the Sprite\n"));
			dprintf(("%d\n", type));
			spriteArray[type]=new Sprite(ENEMY1_FILE_NAME,ENEMY1_FRAMES,ENEMY1_WIDTH,
				ENEMY1_HEIGHT,ENEMY1_ROWS,ENEMY1_COLUMNS);
			spriteArray[type]->loadSprites();
			break;
		default:
			break;
		}
	}
	//dprintf(("Leaving Enemy::Init\n"));
	return true;
}

////////////////////////////////////////////////////////

void Enemy::setSpeed(int speed){
	speedX=speedY=speed;
}

////////////////////////////////////////////////////////

int Enemy::getSpeed(){
	return speedX;
}

////////////////////////////////////////////////////////

SceneCoordinate *Enemy::getCenterPoint(){	
	return new SceneCoordinate(positionX+(spriteArray[type]->width()>>1),
				positionY+(spriteArray[type]->height()>>1));
}

////////////////////////////////////////////////////////

int Enemy::getWidth(){
	return spriteArray[type]->width();
}

////////////////////////////////////////////////////////

int Enemy::getHeight(){
	return spriteArray[type]->height();
}

////////////////////////////////////////////////////////

void Enemy::setPosition(int x, int y){
	positionX=x;
	positionY=y;
}

////////////////////////////////////////////////////////

Direction Enemy::getCurrentDirection(){
	return direction;
}

////////////////////////////////////////////////////////

void Enemy::setAnimationSpeed(int speed){
	animationSpeed=speed;
}

////////////////////////////////////////////////////////

void Enemy::setCurrentAnimation(Direction direct){

	switch(direct){
	case NORTH:
		direction=NORTH;
		currentAnimation=0;
		break;
	case EAST:
		direction=EAST;
		currentAnimation=1;
		break;
	case SOUTH:	
		direction=SOUTH;
		currentAnimation=2;
		break;
	case WEST:
		direction=WEST;
		currentAnimation=3;
		break;
	}
    //currentAnimationFrame=0;
}

////////////////////////////////////////////////////////

void Enemy::checkOnScreen(){
	SceneCoordinate* temp;
	temp = getCenterPoint();
	switch(direction){
	case NORTH:
		if(temp->y < 32){
			setCurrentAnimation(SOUTH);
			currentDirectionCounter=0;
		}
		break;
	case SOUTH:
		if(temp->y > SCREEN_HEIGHT-45){
			setCurrentAnimation(NORTH);
			currentDirectionCounter=0;
		}
		break;
	case EAST:
		if(temp->x > SCREEN_WIDTH-32){
			setCurrentAnimation(WEST);
			currentDirectionCounter=0;
		}
		break;
	case WEST:
		if(temp->x < 32){
			setCurrentAnimation(EAST);
			currentDirectionCounter=0;
		}
		break;
	}
}

////////////////////////////////////////////////////////

void Enemy::move(){
	dprintf(("STATE%d\n", aiState));
	int dx=abs(charX-positionX);
	int dy=abs(charY-positionY);

	switch(aiState){
	case RANDOM:
		//dprintf(("ENTERING THE RANDOM STATE\n"));
		currentDirectionCounter++;
		//dprintf(("dCounter%d\n", currentDirectionCounter));
		if(direction == NORTH){
			positionY-=speedY;
		}
		else if(direction == SOUTH){
			positionY+=speedY;
		}
		else if(direction == EAST){
			positionX+=speedX;
		}
		else if(direction == WEST){
			positionX-=speedX;
		}

		if(currentDirectionCounter == 100){
			currentDirectionCounter=0;
			int randomDirection=rand()%4;
			dprintf(("RANDOM%d\n", randomDirection));
			switch(randomDirection){
			case 0:
				setCurrentAnimation(NORTH);
				break;
			case 1:
				setCurrentAnimation(EAST);
				break;
			case 2:
				setCurrentAnimation(SOUTH);
				break;
			case 3:
				setCurrentAnimation(WEST);
				break;
			}
		}
		checkLineOfSight();
		break;
	case ATTACK:
		dprintf(("ENTERED THE ATTACK STATE\n"));
		currentDirectionCounter++;
		stateCounter++;
		if(direction == NORTH){
			positionY-=speedY;
		}
		else if(direction == SOUTH){
			positionY+=speedY;
		}
		else if(direction == EAST){
			positionX+=speedX;
		}
		else if(direction == WEST){
			positionX-=speedX;
		}

		if(stateCounter == 40){
			dprintf(("ENTERED THE SHOOT STATE\n"));
			stateCounter=0;
			//currentDirectionCounter=0;
			aiState=SHOOT;
		}
		if(currentDirectionCounter%50 == 0){
			currentDirectionCounter=0;
			checkLineOfSight();
		}
		
		break;
	case TRACKING:
		if(stateCounter == 0){
			if(dx > dy){
				if(charX < positionX){
					setCurrentAnimation(WEST);
				}
				else if(charX > positionX){
					setCurrentAnimation(EAST);
				}
			}
			else{
				if (charY < positionY){
					setCurrentAnimation(NORTH);
				}
				else if (charY > positionY){
					setCurrentAnimation(SOUTH);
				}
			}
		}

		stateCounter++;

		if(direction == NORTH){
			positionY-=speedY;
		}
		else if(direction == SOUTH){
			positionY+=speedY;
		}
		else if(direction == EAST){
			positionX+=speedX;
		}
		else if(direction == WEST){
			positionX-=speedX;
		}

		if(stateCounter == 50){
			stateCounter = 0;
		}
		checkLineOfSight();
			//if(abs(dx) > 200 && abs(dy) > 200)
			//	setAIState(RANDOM);
		//}
		//checkLineOfSight();
		break;
	case RETREAT:
		break;
	}
}

////////////////////////////////////////////////////////

void Enemy::moveBack(Direction direction){
	switch(direction){
	case NORTH:
		positionY-=speedY;
		break;
	case SOUTH:
		positionY+=speedY;
		break;
	case EAST:
		positionX+=speedX;
		break;
	case WEST:
		positionX-=speedX;
		break;
	}
}

////////////////////////////////////////////////////////

void Enemy::changeHealth(int change){
	health-=change;
	setAIState(TRACKING);
	setAIState(BEEN_HIT);
	if(health<=0)
		enemyState=DYING;
}

////////////////////////////////////////////////////////

State Enemy::getEnemyState(){
	return enemyState;
}

////////////////////////////////////////////////////////

AIState Enemy::getAIState(){
	return aiState;
}

////////////////////////////////////////////////////////

void Enemy::setAIState(AIState state){
	if(aiState != state){
		currentDirectionCounter=0;
		stateCounter=0;
	}
	if(state == ATTACK)
		setSpeed(5);
	else 
		setSpeed(3);
	prevAiState=aiState;
	aiState=state;
}

////////////////////////////////////////////////////////

bool Enemy::draw(LPDIRECTDRAWSURFACE surface){
		if( !(spriteArray[type]->draw(surface, positionX, positionY, currentFrame)) ){
			dprintf(("Problem in Enemy::draw\n"));
			return FALSE;
		}
	return TRUE;
}

////////////////////////////////////////////////////////

BoundingBox* Enemy::getBoundingBox(){
	return new BoundingBox(positionY, positionX,positionY+spriteArray[type]->height(),
						   positionX+spriteArray[type]->width());
}

////////////////////////////////////////////////////////

BoundingBox* Enemy::getFeetBoundingBox(){
	return new BoundingBox(positionY+(7*(spriteArray[type]->height()>>3)), positionX+1,
		positionY+spriteArray[type]->height(), positionX+spriteArray[type]->width()-1);
}

////////////////////////////////////////////////////////

void Enemy::giveCharacterCoords(int x, int y){
	charX=x;
	charY=y;
	//if(aiState == RANDOM)
	//	checkLineOfSight();
}

////////////////////////////////////////////////////////

void Enemy::checkLineOfSight(){
	int dx=charX-positionX;
	int dy=charY-positionY;

	switch(direction){
	case NORTH:
		if(abs(dx)<20 && dy > -150 && dy < 0)
			setAIState(ATTACK);
		else{
			if(abs(dx) > 200 && abs(dy) > 200)
				setAIState(RANDOM);
			else
				setAIState(TRACKING);
		}
		break;
	case SOUTH:
		if(abs(dx)<20 && dy < 150 && dy > 0)
			setAIState(ATTACK);
		else{
			if(abs(dx) > 200 && abs(dy) > 200)
				setAIState(RANDOM);
			else
				setAIState(TRACKING);
		}
		break;
	case EAST:
		if(abs(dy)<20 && dx < 150 && dx > 0)
			setAIState(ATTACK);
		else{
			if(abs(dx) > 200 && abs(dy) > 200)
				setAIState(RANDOM);
			else
				setAIState(TRACKING);
		} 
		break;
	case WEST:
		if(abs(dy)<20 && dx > -150 && dx < 0)
			setAIState(ATTACK);
		else{
			if(abs(dx) > 200 && abs(dy) > 200)
				setAIState(RANDOM);
			else
				setAIState(TRACKING);
		} 
		break;
	}
}

////////////////////////////////////////////////////////

SceneCoordinate *Enemy::getArmPosition(){
	switch(direction){
	case NORTH:
		return new SceneCoordinate(positionX-10, positionY);
		break;
	case EAST:
		return new SceneCoordinate(positionX+spriteArray[type]->width()-30, positionY+25);
		break;
	case SOUTH:
		return new SceneCoordinate(positionX+spriteArray[type]->width(), positionY+spriteArray[type]->height()-30);
		break;
	case WEST:
		return new SceneCoordinate(positionX, positionY+20);
		break;
	default:
		return new SceneCoordinate(positionX, positionY);
		break;
	}
}

////////////////////////////////////////////////////////

bool Enemy::animate(){
	if(enemyState == DYING){
		if((GetTickCount()-lastDeadTime) > 100){
			lastDeadTime=GetTickCount();
			if(currentDeadFrame == 2){
				//currentDeadFrame=0;
				enemyState=DEAD;
			}
			else{
				currentFrame=deadAnimationArray[currentAnimation][currentDeadFrame];
				currentDeadFrame++;
			}
		}
	}
	
	if((GetTickCount()-lastMoveTime) > animationSpeed && enemyState != DEAD){
		if(aiState==SHOOT){
			if((GetTickCount()-lastShootTime) > 100){
				lastShootTime=GetTickCount();
				if(currentShootFrame == 2){
					currentShootFrame=0;
					aiState=ATTACK;
					currentFrame=animationArray[currentAnimation][0];
				}
				else{
					currentFrame=shootArray[currentAnimation][currentShootFrame];
					currentShootFrame++;
				}
			}
		}
		else if(aiState==BEEN_HIT){
			if((GetTickCount()-lastHitTime) > 100){
				lastHitTime=GetTickCount();
				if(currentHitFrame == 2){
					currentHitFrame=0;
					//setAIState(ATTACK);
					aiState=prevAiState;
					currentFrame=animationArray[currentAnimation][0];
				}
				else{
					currentFrame=hitArray[currentAnimation][currentHitFrame];
					currentHitFrame++;
				}
			}
		}
		else{
			animationCounter=0;
			lastMoveTime=GetTickCount();
			currentAnimationFrame++;

			if(currentAnimationFrame>3)
				currentAnimationFrame=0;

			currentFrame=animationArray[currentAnimation][currentAnimationFrame];
		}
	}
	return TRUE;
}

bool Enemy::restore(){
	if(spriteArray[type]->surfaceLost() == true){
		spriteArray[type]->restore();
		spriteArray[type]->load(ENEMY1_FILE_NAME);
		spriteArray[type]->loadSprites();
	}
	return true;
}

void Enemy::release(){

}