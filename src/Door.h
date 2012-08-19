// Door.h: interface for the Door class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DOOR_H_
#define _DOOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Entity.h"
#include "defines.h"

typedef enum{DOOR_OPEN, DOOR_CLOSED} DoorState;

class BmpLoader;
class SceneCoordinate;

class Door : public Entity{
public:
	Door(int width, int height);
	virtual ~Door();
	void toggle();  // if open then close and vice versa
	virtual bool draw(LPDDSurface surface) = 0;
	virtual bool init();
	virtual BoundingBox *getBaseBoundingBox();
protected:	
	DoorState state;
	int currFrame;
};

typedef list<Door *> DoorList;

#endif // _DOOR_H
