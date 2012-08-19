// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "Action.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Action::Action(ActionType type, Entity *source, ActionCallback callback, void *data){
	this->type = type;
	this->source = source;
	this->callback = callback;
	this->data = data;
}


Action::~Action(){

}
