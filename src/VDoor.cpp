// VDoor.cpp: implementation of the VDoor class.
//
//////////////////////////////////////////////////////////////////////

#include "VDoor.h"
#include "files.h"
#include "SceneCoordinate.h"
#include "BoundingBox.h"
#include "BmpLoader.h"
#include "debug.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VDoor::VDoor(int width, int height) : Door(width, height){

}

VDoor::~VDoor(){

}

bool VDoor::draw(LPDDSurface surface){
	
	if(state == DOOR_CLOSED && currFrame > 0) currFrame--;
	else if(state == DOOR_OPEN && currFrame < V_DOOR_FRAMES-1) currFrame++;	

	RECT destRect;
	destRect.top = sceneCoord->y;
	destRect.left = sceneCoord->x;
	destRect.bottom = sceneCoord->y + V_DOOR_HEIGHT;
	destRect.right = sceneCoord->x + V_DOOR_WIDTH;

	BoundingBox *srcBox = new BoundingBox( 1, (currFrame*V_DOOR_WIDTH)+currFrame+1, 
		V_DOOR_HEIGHT+1, (currFrame*(V_DOOR_WIDTH+1))+V_DOOR_WIDTH+1);	
		
	return bmp->drawBox(surface, getBoundingBox(), srcBox, DDBLT_KEYSRC|DDBLT_WAIT);
}
