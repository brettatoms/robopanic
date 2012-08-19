// Entity.h: interface for the Entity class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ENTITY_H_
#define _ENTITY_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <list>
#include <ddraw.h>
#include <dsound.h>
#include "defines.h"
#include "Action.h"

using namespace std;  // for STL

// so we don't have headers in headers
class BmpLoader;
class SceneCoordinate;
class Action;
class BoundingBox;

class Entity{
public:
	Entity(int width, int height);
	virtual ~Entity();
	virtual bool draw(LPDDSurface surface);	 // draw at location sceneCoord
	virtual bool draw(LPDDSurface surface, int destX, int destY);	// draw at destX, destY
	virtual bool load();
	char *bmpFileName;
	bool playSound();
	void release();
	bool restore();
	void unload();
	virtual bool init();
	bool isInitialized(){ return initialized; };
	void addAction(ActionType type, ActionCallback func);
	ActionList getActionList();

	int getX();  // return sceneCoord->x;
	int getY();  // return sceneCoord->y	
	SceneCoordinate *getSceneCoord();
	void getSceneCoord(int &x, int &y);	
	void setSceneCoord(SceneCoordinate coord);
	void setSceneCoord(int x, int y);	
	bool isCoordInside(int x, int y);
	virtual BoundingBox *getBoundingBox();
	virtual BoundingBox *getBaseBoundingBox();
	int getWidth(){ return width; }
	int getHeight(){ return height; }
	bool getAttribute(int index);
	void setAttribute(int index, bool value);
	SceneCoordinate *getCenterCoord();
	LPDSoundBuffer sound;
	//long width, height;
	char *soundFileName;
	ActionList actionList; //action that can be performed on this entity
protected:	
	long width, height;
	bool initialized;	
	char *description;	
	bool attributes[NUM_ENTITY_ATTRIBS];
	SceneCoordinate *sceneCoord;	
	BmpLoader *bmp;
	
};

typedef list<Entity *> EntityList;

#endif // _ENTITY_H_
