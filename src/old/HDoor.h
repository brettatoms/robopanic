// HDoor.h: interface for the HDoor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HDOOR_H_
#define _HDOOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Door.h"

class BoundingBox;

// an implementation of a horizontal door
class HDoor : public Door {
public:
	HDoor(int width, int height);
	virtual ~HDoor();
	virtual bool draw(LPDDSurface surface);
	BoundingBox *getBaseBoundingBox(){ return getBoundingBox(); }
};

#endif // _HDOOR_H_
