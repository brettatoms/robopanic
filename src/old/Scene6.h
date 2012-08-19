// Scene6.h: interface for the Scene6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE6_H__8E4B017E_91BA_47E3_919C_34687690B435__INCLUDED_)
#define AFX_SCENE6_H__8E4B017E_91BA_47E3_919C_34687690B435__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scene.h"

class Scene6 : public Scene  
{
public:
	Scene6(char *dataFile);
	bool init();
	virtual ~Scene6();
	void addEnemy();
	bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
};

#endif // !defined(AFX_SCENE6_H__8E4B017E_91BA_47E3_919C_34687690B435__INCLUDED_)
