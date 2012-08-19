// Scene1.h: interface for the Scene1 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SCENE1_H_
#define SCENE1_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Scene.h"

class Entity;

class Scene1 : public Scene{
public:
	Scene1(char *dataFile);
	virtual ~Scene1();
	bool load();
	bool init();
	bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
	void addEnemy();
protected:
	//void activateSwitch(Entity *source, Action *action);
};

#endif // SCENE1_H_
