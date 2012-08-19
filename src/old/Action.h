// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ACTION_H_
#define _ACTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "defines.h"

using namespace std;

class Action;
class Entity;

//typedef void (*ActionCallback)(Entity *source, Action *action, void *data);
typedef void (*ActionCallback)(Action *action);

class Action{
	
public:
	Action(ActionType type, Entity *source, ActionCallback callback, void *data = NULL);
	//Action(ActionType type, ActionCallback callback, void *data);
	virtual ~Action();
	//ActionType getType(){ return type; };
	//void react(Entity *source){ callback(source, this, data); };	
	void react(){ callback(this); };	
	ActionType type;
	void *data;
	Entity *source;
protected:
	//void react(){ callback(this); };	
	ActionCallback callback;
	
};

typedef list<Action *> ActionList;

#endif // _ACTION_H_
