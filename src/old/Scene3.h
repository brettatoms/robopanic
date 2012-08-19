// Scene3.h: interface for the Scene3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE3_H__4AD519F9_D05A_459F_B975_929582CDA998__INCLUDED_)
#define AFX_SCENE3_H__4AD519F9_D05A_459F_B975_929582CDA998__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scene.h"

class Scene3 : public Scene {
public:
	Scene3(char *dataFile);
	bool init();
	virtual ~Scene3();
	void addEnemy();
	bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
};

#endif // !defined(AFX_SCENE3_H__4AD519F9_D05A_459F_B975_929582CDA998__INCLUDED_)
