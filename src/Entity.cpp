// Entity.cpp: implementation of the Entity class.
//
//////////////////////////////////////////////////////////////////////

#include "Entity.h"
#include "SceneCoordinate.h"
#include "debug.h"
//#include "Action.h"
#include "BmpLoader.h"
#include "BoundingBox.h"
#include "sound.h"

extern LPDirectDraw lpDirectDrawObject;
extern CSoundManager *soundManager;

/////////////////////////////////////////////////////

Entity::Entity(int width, int height){
	bmp = NULL;
	sceneCoord = NULL;
	bmpFileName = NULL;
	soundFileName = NULL;
	sound = NULL;
	this->width = width;
	this->height = height;	
	initialized = false;
	for(int i=0; i<NUM_ENTITY_ATTRIBS; i++)  attributes[i] = false;
}

/////////////////////////////////////////////////////

Entity::~Entity(){
	release();
	delete bmp;
	delete sceneCoord;
	delete bmpFileName;
	delete soundFileName;
	for(ActionList::iterator i = actionList.begin(); i != actionList.end(); i++)
		delete (*i);
}

/////////////////////////////////////////////////////

void Entity::release(){
	bmp->release();
	sound->Stop();
	sound->Release();	
}

/////////////////////////////////////////////////////

void Entity::unload(){
	return;
}

/////////////////////////////////////////////////////

bool Entity::isCoordInside(int x, int y){
	if(x >= sceneCoord->x && x < sceneCoord->x+width-1 && 
		y >= sceneCoord->y && y < sceneCoord->y+height-1)
		return TRUE;
	else return FALSE;
}

/////////////////////////////////////////////////////

bool Entity::getAttribute(int index){ return attributes[index]; }
void Entity::setAttribute(int index, bool value){ attributes[index] = value; }


/////////////////////////////////////////////////////

bool Entity::playSound(){
	dprintf(("entered Entity::playSound()\n"));
	if(sound == NULL) return false;	
	soundManager->play(sound, false);
	dprintf(("leaving Entity::playSound()\n"));
	return true;
}


/////////////////////////////////////////////////////


void Entity::getSceneCoord(int &x, int &y){
	x = sceneCoord->x;
	y = sceneCoord->y;
}

SceneCoordinate *Entity::getSceneCoord(){
	return new SceneCoordinate(sceneCoord->x, sceneCoord->y);
	//coord = (*sceneCoord);
}

/////////////////////////////////////////////////////

// need to know the size and location of the image on the
// contact sheet
bool Entity::load(){
	//dprintf(("entered Entity::load()\n"));
	if(!initialized) return false;
	//dprintf(("leaving Entity::load()\n"));	
	return true;
}

/////////////////////////////////////////////////////

bool Entity::init(){
	dprintf(("entered Entity::init()\n"));

	// init image
	if(bmpFileName != NULL){
		bmp = new BmpLoader();		
		if(!bmp->load(bmpFileName)) {
			dprintf(("  problem loading bmp\n"));		
			return FALSE;
		}
	}

	if(soundFileName != NULL)
		sound = soundManager->createBuffer(soundFileName);

	if(sound == NULL){
		dprintf(("*********** cound not init sound %s\n", soundFileName));
	}
	else dprintf(("*********** inited sound %s\n", soundFileName));


	dprintf(("leaving Entity::init()\n"));
	
	// init actions	
	initialized = true;
	return true;
}


BoundingBox *Entity::getBoundingBox(){
	return new BoundingBox(sceneCoord->y, sceneCoord->x, 
		sceneCoord->y+height, sceneCoord->x+width);
}


BoundingBox *Entity::getBaseBoundingBox(){	
	
	return new BoundingBox(sceneCoord->y+(7*(height/8)), sceneCoord->x, 
						   sceneCoord->y+height, sceneCoord->x+width);
}

/////////////////////////////////////////////////////

void unload(){
	return;
}

/////////////////////////////////////////////////////

int Entity::getX(){ return sceneCoord->x; };
int Entity::getY(){ return sceneCoord->y; };

/////////////////////////////////////////////////////

// draw at destX and destY but do not changethe Entity's sceneCoord
bool Entity::draw(LPDDSurface surface, int destX, int destY){
	RECT destRect;
	destRect.top = destY;
	destRect.left = destX;
	destRect.bottom = destY+height;
	destRect.right = destX+width;
	
	return bmp->draw(surface, destX, destY, DDBLT_KEYSRC | DDBLT_WAIT);
}

/////////////////////////////////////////////////////

SceneCoordinate *Entity::getCenterCoord(){
	//return new SceneCoordinate((sceneCoord->x + width)>>1, (sceneCoord->y + height)>>1);	
	return new SceneCoordinate((sceneCoord->x + (sceneCoord->x+width))>>1, 
		(sceneCoord->y + (sceneCoord->y+height))>>1);	
}


/////////////////////////////////////////////////////

bool Entity::draw(LPDDSurface surface){
	//dprintf(("entering Entity::draw()\n"));
	
	RECT destRect;
	destRect.top = sceneCoord->y;
	destRect.left = sceneCoord->x;
	destRect.bottom = sceneCoord->y+height;
	destRect.right = sceneCoord->x+width;
	BoundingBox *box = new BoundingBox(sceneCoord->y, sceneCoord->x, 
		sceneCoord->y+height, sceneCoord->x+width);
	return bmp->drawBox(surface, box, NULL, DDBLT_KEYSRC | DDBLT_WAIT);
}

/////////////////////////////////////////////////////

ActionList Entity::getActionList(){
	return actionList;
}

/////////////////////////////////////////////////////


void Entity::setSceneCoord(int x, int y){	
	if(!sceneCoord) sceneCoord = new SceneCoordinate(x, y);
	else{ sceneCoord->x = x;  sceneCoord->y = y; }
}


bool Entity::restore(){
	dprintf(("Entering the Entity::restore\n"));
	dprintf(("%s", bmpFileName));
	if(bmpFileName != NULL){
		if(bmp->restore())
			bmp->load(bmpFileName);
	}
	return true;
}