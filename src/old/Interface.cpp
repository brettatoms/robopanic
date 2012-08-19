// Interface.cpp: implementation of the Interface class.
//
//////////////////////////////////////////////////////////////////////

#include "Interface.h"
#include "BmpLoader.h"
#include "files.h"
#include "defines.h"
#include <ddraw.h>
#include "Action.h"
#include "ActionMenu.h"
#include "Entity.h"
#include "World.h"
#include "Character.h"
#include "debug.h"

bool createOffScreenSurface(LPDDSurface &surface, int width, int height);
extern LPDDSurface lpSecondary;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Interface::Interface(){
	actionMenu = NULL;
	/*
	healthBar.top=inHealthBar.top=20;
	healthBar.bottom=inHealthBar.bottom=30;
	healthBar.left=2;
	inHealthBar.right=102;

	outHealthBar.top=18;
	outHealthBar.left=0;
	outHealthBar.bottom=32;
	outHealthBar.right=104;
	*/
	healthBar.top=inHealthBar.top=20;
	healthBar.bottom=inHealthBar.bottom=30;
	healthBar.left=32;
	inHealthBar.right=132;

	outHealthBar.top=18;
	outHealthBar.left=30;
	outHealthBar.bottom=32;
	outHealthBar.right=134;
}

Interface::~Interface(){
	initialized = false;	
	delete actionMenu;
}

bool Interface::draw(LPDDSurface surface, World *world){
	//dprintf(("entered Interface::draw()\n"));
	//HRESULT r = DD_OK;
	drawHealth(surface, world->character->getHealth());
	if(actionMenu->isActive())
		if(!actionMenu->draw(surface)) return false;

	//dprintf(("leaving Interface::draw()\n"));
	return true;
}

void Interface::drawHealth(LPDDSurface surface, int health){
	drawText("Health",0,0,RGB(0,0,0),surface);
	DDBLTFX fx; ZeroMemory(&fx, sizeof(fx));	
	fx.dwSize = sizeof(fx);

	fx.dwFillColor = RGB(30,30,30); 
	surface->Blt(&outHealthBar, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
	
	fx.dwFillColor = 60000;
	healthBar.right=healthBar.left+health;
	surface->Blt(&healthBar, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
	
	fx.dwFillColor = RGB(1,1,1);
	inHealthBar.left=healthBar.right;
	surface->Blt(&inHealthBar, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
}

void Interface::drawText(char *text,int x,int y,COLORREF color,LPDDSurface surface){
HDC xdc; // the working dc
	// get the dc from surface
	if (FAILED(surface->GetDC(&xdc)))
		dprintf(("Interface::drawText\n"));
	// set the colors for the text up
	SetTextColor(xdc, color);
	// set background mode to transparent so black isn't copied
	SetBkMode(xdc, TRANSPARENT_COLOR);
	// draw the text a
	TextOut(xdc,x,y,text,strlen(text));
	// release the dc
	surface->ReleaseDC(xdc);
}	

bool Interface::init(){
	// initialize buttons for action menus
	actionMenu = new ActionMenu();
	if(!actionMenu->init()) return false;
	
	// more
	initialized = true;
	return true;
}

void Interface::hideActionMenu(){
	if(!initialized) return;
	actionMenu->setActive(false);	
}	

//bool Interface::createActionMenu(Entity *entity){//, int x, int y){
bool Interface::showActionMenu(Entity *entity, int x, int y){
	dprintf(("entered Interface::showActionMenu()\n"));
	if(!initialized) return false;
	// if entity == NULL then draw the menu that is already created
	if(entity != NULL)	actionMenu->constructMenu(entity);
	actionMenu->setCoord(x, y);	
	actionMenu->setActive(true);
	return true;

}

void Interface::release(){
	if(actionMenu) actionMenu->release();
}

