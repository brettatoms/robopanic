// HDoor.cpp: implementation of the HDoor class.
//
//////////////////////////////////////////////////////////////////////

#include "HDoor.h"
#include "files.h"
#include "SceneCoordinate.h"
#include "BoundingBox.h"
#include "BmpLoader.h"
#include "debug.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HDoor::HDoor(int width, int height) : Door(width, height){

}

HDoor::~HDoor(){

}


bool HDoor::draw(LPDDSurface surface){
	
	if(state == DOOR_CLOSED && currFrame > 0) currFrame--;
	else if(state == DOOR_OPEN && currFrame < H_DOOR_FRAMES-1) currFrame++;	

	RECT destRect;
	destRect.top = sceneCoord->y;
	destRect.left = sceneCoord->x;
	destRect.bottom = sceneCoord->y + H_DOOR_HEIGHT;
	destRect.right = sceneCoord->x + H_DOOR_WIDTH;

	BoundingBox *srcBox = new BoundingBox((currFrame*H_DOOR_HEIGHT)+currFrame+1,
		1, (currFrame*(H_DOOR_HEIGHT+1))+H_DOOR_HEIGHT+1, H_DOOR_WIDTH+1);	
		
	return bmp->drawBox(surface, getBoundingBox(), srcBox, DDBLT_KEYSRC|DDBLT_WAIT);
}