// ActionMenu.h: interface for the ActionMenu class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ACTIONMENU_H_
#define _ACTIONMENU_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "defines.h"
#include <ddraw.h>

class BmpLoader;
class Entity;
class SceneCoordinate;

class ActionMenu{
public:
	ActionMenu();
	virtual ~ActionMenu();
	bool draw(LPDDSurface surface, int x, int y);
	bool draw(LPDDSurface surface);
	bool init();
	bool constructMenu(Entity *entity);
	bool isActive(){ return active; };
	void setActive(bool active){ this->active = active; };
	void setCoord(int x, int y);
	void release();
	//int getActionTypeAt(int x, int y);
	int getItemAt(int x, int y);
protected:
	int width, height;
	Entity *actionReciever;	
	BmpLoader *bmp;
	SceneCoordinate *sceneCoord;
	LPDDSurface image;
	int x, y;
	bool active;
	bool initialized;
};

#endif // _ACTIONMENU_H_
