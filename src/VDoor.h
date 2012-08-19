// VDoor.h: interface for the VDoor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _VDOOR_H_
#define _VDOOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Door.h"

class VDoor : public Door{
public:
	VDoor(int width, int height);
	virtual ~VDoor();
	bool draw(LPDDSurface surface);	
};

#endif // _VDOOR_H_
