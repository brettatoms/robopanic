// Inventory.h: interface for the Inventory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "Entity.h"

using namespace std; // for STL

class Entity;

class Inventory {
public:
	Inventory();
	virtual ~Inventory();
	
	void addItem(Entity *entity);
	bool removeItem(Entity *entity);
	bool removeItem(int index);
	Entity *getItem(int index);
	
protected:	
	EntityList inventoryItems;
};

#endif 
