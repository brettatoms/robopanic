// SceneCoordinate.h: interface for the SceneCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCENECOORDINATE_H_
#define _SCENECOORDINATE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <list>

using namespace std;

class SceneCoordinate {
public:
	//SceneCoordinate();
	SceneCoordinate(int x=0, int y=0);

	virtual ~SceneCoordinate();

	// convert scene coords x and y to screen coords and place the values
	// back into the passed variables
	//static void sceneToScreenCoords(int &x, int &y); 

	// convert screen coords x and y to scene coords and place the values
	// back into the passed variables
	//static void screenToSceneCoords(int &x, int &y);	

	int x, y;
};

typedef list<SceneCoordinate *> SceneCoordinateList;

#endif // _SCENECOORDINATE_H_
