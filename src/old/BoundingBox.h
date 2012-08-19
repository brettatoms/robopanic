// BoundingBox.h: interface for the BoundingBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ddraw.h>   // for RECT

class SceneCoordinate;

// an axis aligned bounding box, i.e. only works if x and y axis
// of box are parallel with the natural x and y axis
class BoundingBox {
public:	
	//BoundingBox();
	BoundingBox(int top, int left, int bottom, int right);
	virtual ~BoundingBox();
	const bool overlaps(const BoundingBox &box) const;
	//const bool edgeOverlaps(const BoundingBox &box) const;
	SceneCoordinate *center;	// center postion 
	SceneCoordinate *extents;	// extents from center	
	RECT *toRECT(); // return this as a windows defines rect
	int getTop(){ return top; }
	int getLeft(){ return left; }
	int getBottom(){ return bottom; }
	int getRight(){ return right; }
	int top, left, bottom, right;   // if these are changed then the center and extents
									// will be wrong and the overlaps function
									// wont work
protected:

};

#endif // _BOUNDINGBOX_H_
