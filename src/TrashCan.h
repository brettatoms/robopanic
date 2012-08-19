// TrashCan.h: interface for the TrashCan class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TRASHCAN_H_
#define _TRASHCAN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Entity.h"

class Action;

class TrashCan : public Entity{
public:
	TrashCan();
	virtual ~TrashCan();
	bool init();
	ActionCallback dummy;
	//void* dummy(Action *action);

};

#endif // _TRASHCAN_H_
