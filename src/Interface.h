// Interface.h: interface for the Interface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ddraw.h>
#include "defines.h"

class World;
class Entity;
class BmpLoader;
class ActionMenu;

class Interface{
public:
	Interface();
	virtual ~Interface();	
	bool showActionMenu(Entity *entity, int x, int y);
	void hideActionMenu();	
	bool init();
	bool draw(LPDDSurface surface, World *world);	
	int getSelectedAction(int x, int y); // returns -1 for no action selected
	ActionMenu *actionMenu;	
	void drawHealth(LPDDSurface surface, int health);
	void drawText(char *text,int x,int y,COLORREF color,LPDDSurface surface);
	void release();
protected:
	bool initialized;		
	RECT healthBar;
	RECT outHealthBar;
	RECT inHealthBar;
};

#endif // _INTERFACE_H_
