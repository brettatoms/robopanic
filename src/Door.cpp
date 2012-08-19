// Door.cpp: implementation of the Door class.
//
//////////////////////////////////////////////////////////////////////

#include "Door.h"
#include "Entity.h"
#include "SceneCoordinate.h"
#include "files.h"
#include "BoundingBox.h"
#include "BmpLoader.h"

#include "debug.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Door::Door(int width, int height) : Entity(width, height){
	currFrame = 0;
	state = DOOR_CLOSED;	
	setAttribute(ENTITY_WALKTHRU, false);
}

Door::~Door(){

}


bool Door::init(){
	if(!(this->Entity::init())) return false;	
	return true;
}


void Door::toggle(){
	if(state == DOOR_OPEN){
		state = DOOR_CLOSED;
		setAttribute(ENTITY_WALKTHRU, false);
	}
	else{
		state = DOOR_OPEN;
		setAttribute(ENTITY_WALKTHRU, true);
	}
}

BoundingBox *Door::getBaseBoundingBox(){
	return getBoundingBox();
}

/*
bool Door::draw(LPDDSurface surface){
	
	if(state == DOOR_CLOSED && currFrame > 0) currFrame--;
	else if(state == DOOR_OPEN && currFrame < DOOR_FRAMES-1) currFrame++;	

	RECT destRect;
	destRect.top = sceneCoord->y;
	destRect.left = sceneCoord->x;
	destRect.bottom = sceneCoord->y + H_DOOR_HEIGHT;
	destRect.right = sceneCoord->x + H_DOOR_WIDTH;

	BoundingBox *srcBox = new BoundingBox((currFrame*H_DOOR_HEIGHT)+currFrame+1,
		1, (currFrame*H_DOOR_HEIGHT)+H_DOOR_HEIGHT+1, H_DOOR_WIDTH+1);	
		
	return bmp->drawBox(surface, getBoundingBox(), srcBox, DDBLT_KEYSRC|DDBLT_WAIT);
}
*/