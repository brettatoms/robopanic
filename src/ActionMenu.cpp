// ActionMenu.cpp: implementation of the ActionMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "ActionMenu.h"
#include <ddraw.h>
#include "files.h"
#include "Entity.h"
#include "SceneCoordinate.h"
#include "BmpLoader.h"
#include "debug.h"

bool createOffScreenSurface(LPDDSurface &surface, int width, int height);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ActionMenu::ActionMenu(){
	image = NULL;
	sceneCoord = NULL;
	bmp = NULL;
	initialized = false;
	active = false;
	width = height = -1;
}

ActionMenu::~ActionMenu(){
	release();
	delete sceneCoord;
	delete bmp;
}

void ActionMenu::release(){
	if(image) image->Release();
	image = NULL;
}

bool ActionMenu::init(){
	if(initialized) return false;

	bmp = new BmpLoader();
	bmp->load(ACTION_BUTTON_FILE_NAME);

	sceneCoord = new SceneCoordinate();
	initialized = true;
	return true;
}

bool ActionMenu::draw(LPDDSurface surface, int x, int y){
	setCoord(x, y);
	return draw(surface);	
}

bool ActionMenu::draw(LPDDSurface surface){	
	RECT destRect;
	destRect.top = sceneCoord->y;
	destRect.left = sceneCoord->x;
	destRect.bottom = sceneCoord->y+height;
	destRect.right= sceneCoord->x+width;
	HRESULT r = surface->Blt(&destRect, image, NULL, DDBLT_WAIT, NULL);
	return ((r == DD_OK) ? true : false);
}

void ActionMenu::setCoord(int x, int y){
	if(sceneCoord == NULL) sceneCoord = new SceneCoordinate(x, y);
	else{ sceneCoord->x = x;	sceneCoord->y = y; }
}


// return the index number of the button at x,y
// a -1 means no button
int ActionMenu::getItemAt(int x, int y){
	if(x < sceneCoord->x || x > sceneCoord->x+width || 
		y < sceneCoord->y || y > sceneCoord->y+height) return -1;
	return (y-sceneCoord->y)/(ACTION_BUTTON_HEIGHT+1);
	
	
}

bool ActionMenu::constructMenu(Entity *entity){
	dprintf(("entered Interface::constructActionMenu()\n"));

	if(!initialized) return false;

	//destroyActionMenu(); // kill any old action menu	
	release();

	ActionList actions = entity->getActionList();
			
	if(actions.size() <= 0){
		dprintf(("  size <= 0\n"));
		return false;
	}		
				
	width = ACTION_BUTTON_WIDTH;
	height = actions.size()*ACTION_BUTTON_HEIGHT;
	if(!createOffScreenSurface(image, width, height )) return false;
	//if(!createOffScreenSurface(image, ACTION_BUTTON_WIDTH, 
		//actions.size()*ACTION_BUTTON_HEIGHT)) return false;		
	if(image== NULL){ dprintf(("  surface == NULL\n")); }

	ActionList::iterator i;	
	int j;		
	for(j=0, i=actions.begin(); j<actions.size(); i++, j++){			
		//ActionType type = (*i)->getType();
		ActionType type = (*i)->type;
		int srcX = ((type%ACTION_BUTTON_FILE_COLUMNS)*(ACTION_BUTTON_WIDTH+1))+1;
		int srcY = ((type/ACTION_BUTTON_FILE_ROWS)*(ACTION_BUTTON_HEIGHT+1))+1;			
		if(!bmp->drawRectTo(0, j*ACTION_BUTTON_HEIGHT, image, srcX, srcY, 
			ACTION_BUTTON_WIDTH, ACTION_BUTTON_HEIGHT)){
			dprintf(("  problem drawing action button %d, type: %d\n", j, type));
			//destroyActionMenu();
			return false;
		}
	}

	actionReciever = entity;
	return true;
}