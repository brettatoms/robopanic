// InvisibleEntity.cpp: implementation of the InvisibleEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "BoundingBox.h"
#include "InvisibleEntity.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InvisibleEntity::InvisibleEntity(int width, int height): Entity(width, height)
{
	
}

InvisibleEntity::~InvisibleEntity()
{

}


bool InvisibleEntity::draw(LPDDSurface surface){
	return true;
}



bool InvisibleEntity::draw(LPDDSurface surface, int destX, int destY){
	return true;
}


BoundingBox *InvisibleEntity::getBaseBoundingBox(){	
	return getBoundingBox();
}

bool InvisibleEntity::init(){
	return this->Entity::init();
	//return initialized = true;
}