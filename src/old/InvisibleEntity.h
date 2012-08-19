// InvisibleEntity.h: interface for the InvisibleEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVISIBLEENTITY_H__F4931EE1_AE77_11D3_8A6E_00902793B09B__INCLUDED_)
#define AFX_INVISIBLEENTITY_H__F4931EE1_AE77_11D3_8A6E_00902793B09B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Entity.h"

class InvisibleEntity : public Entity  
{
public:
	InvisibleEntity(int width, int height);
	bool init();
	virtual ~InvisibleEntity();
	bool draw(LPDDSurface surface);	
	bool draw(LPDDSurface surface, int destX, int destY);	
	BoundingBox* getBaseBoundingBox();	
	

};

#endif // !defined(AFX_INVISIBLEENTITY_H__F4931EE1_AE77_11D3_8A6E_00902793B09B__INCLUDED_)
