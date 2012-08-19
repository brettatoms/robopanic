// Scene5.h: interface for the Scene5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE5_H__8DCD3D8D_9759_4E2C_8C4A_7B79A9F6344C__INCLUDED_)
#define AFX_SCENE5_H__8DCD3D8D_9759_4E2C_8C4A_7B79A9F6344C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scene.h"

class Scene5 : public Scene  
{
public:
	Scene5(char *dataFile);
	bool init();
	virtual ~Scene5();
	void addEnemy();
	bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
};

#endif // !defined(AFX_SCENE5_H__8DCD3D8D_9759_4E2C_8C4A_7B79A9F6344C__INCLUDED_)
