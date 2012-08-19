// Scene7.h: interface for the Scene7 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE7_H__6B5CBFB4_0DFA_4D25_A4D8_2CD0F5DA4617__INCLUDED_)
#define AFX_SCENE7_H__6B5CBFB4_0DFA_4D25_A4D8_2CD0F5DA4617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scene.h"

class Scene7 : public Scene  
{
public:
	Scene7(char *dataFile);
	bool init();
	virtual ~Scene7();
	void addEnemy();
	bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
};

#endif // !defined(AFX_SCENE7_H__6B5CBFB4_0DFA_4D25_A4D8_2CD0F5DA4617__INCLUDED_)
