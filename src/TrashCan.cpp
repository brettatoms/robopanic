// TrashCan.cpp: implementation of the TrashCan class.
//
//////////////////////////////////////////////////////////////////////

#include "TrashCan.h"
#include "Action.h"
#include "debug.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TrashCan::TrashCan() : Entity(32, 64){
	width = 32; 
	height = 64;	
}

TrashCan::~TrashCan(){

}

// member functions

bool TrashCan::init(){
	if(!(this->Entity::init())) return false;
	dprintf(("  insert action\n"));
	actionList.insert(actionList.end(), new Action(ACTION_LOOKAT, this, dummy));
	actionList.insert(actionList.end(), new Action(ACTION_PICKUP, this, dummy));	
	dprintf(("    ** OK\n"));
	return true;
}

ActionCallback dummy(){
	return NULL;
}

//void *TrashCan::dummy(Action *action){
//	return;
//}