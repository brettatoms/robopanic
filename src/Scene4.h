// Scene4.h: interface for the Scene4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE4_H__8AD5728B_982B_44F5_9344_D0936749FB48__INCLUDED_)
#define AFX_SCENE4_H__8AD5728B_982B_44F5_9344_D0936749FB48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scene.h"

class Scene4 : public Scene  
{
public:
	Scene4(char *dataFile);
	bool checkPuzzle();
	bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
	virtual ~Scene4();
	void addEnemy();
	bool init();
private:
	bool puzzle;
	BoundingBox* square1;

};

#endif // !defined(AFX_SCENE4_H__8AD5728B_982B_44F5_9344_D0936749FB48__INCLUDED_)
