// BoundingBox.cpp: implementation of the BoundingBox class.
//
//////////////////////////////////////////////////////////////////////

#include "BoundingBox.h"
#include "SceneCoordinate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BoundingBox::BoundingBox(int top, int left, int bottom, int right){
	center = new SceneCoordinate(left+((right-left)>>1), top+((bottom-top)>>1));
	extents = new SceneCoordinate(center->x-left, center->y-top);		
	this->top = top; 
	this->left = left;
	this->bottom = bottom;
	this->right = right;
}

BoundingBox::~BoundingBox(){
	if(center) delete center;
	if(extents) delete extents;
}


RECT *BoundingBox::toRECT(){
	RECT *r = new RECT; 
	r->top = top; r->left = left; r->bottom = bottom; r->right = right;	
	return r;
}

// return true if <box> intersects/overlaps this
const bool BoundingBox::overlaps(const BoundingBox &box) const{	
	SceneCoordinate diff(abs(center->x-box.center->x), abs(center->y-box.center->y));
	return (diff.x <= extents->x+box.extents->x) && (diff.y <= extents->y+box.extents->y);	
}

