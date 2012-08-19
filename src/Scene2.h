// Scene2.h: interface for the Scene2 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCENE2_H_
#define _SCENE2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scene.h"

class Scene2 : public Scene{
public:
	Scene2(char *sceneDataFileName);
	Scene2();
	bool init();
	virtual ~Scene2();
	void addEnemy();
	bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
};

#endif // _SCENE2_H_
