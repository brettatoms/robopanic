#ifndef _SCENE_H
#define _SCENE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <list>
#include "defines.h"
#include <ddraw.h>
#include "Entity.h"
#include "Enemy.h"

using namespace std; 

class TileMap;
class Character;
class Door;

typedef list<LPDDSurface> LPDDSurfaceList;

class Scene{
public:	
	Scene(char *dataFile);
	virtual	~Scene();
	virtual bool init();
	virtual bool load();
	virtual bool FC draw(LPDDSurface surface);
	virtual bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
	//virtual bool FC draw(LPDDSurface surface, int destX, int destY);	
	virtual void unload();		
	
	void getPalette(LPDDPalette palette);
	bool getNeighbor(Direction d, Scene **scene);
	void setNeighbors(Scene *north, Scene *south, Scene *east, Scene *west);
	
	int getEntitiesAt(int x, int y, EntityList *elist);	
	int getEntitiesInBox(BoundingBox *box, EntityList *elist);
	void release();
	virtual bool restore();
	bool isInitialized(){ return initialized; };
	void giveCharacter(Character **character);
	void removeCharacter();
	virtual void addEnemy(){return;}
	
	EntityList entities;	
	EnemyList enemies;	
	TileMap *tileMap;
	bool playBackgroundSound();
	char *backgroundSoundFileName;
	bool isCompleted(){ return completed; }
	bool hasEnemies;
	LPDSoundBuffer sound;
protected:	
	
	bool completed;
	Character *character;
	Scene *neighbors[NUM_DIRECTIONS];	
	Door *doors[NUM_DIRECTIONS];	
	bool initialized;
	int width, height; // in pixels	y
	char *tileMapName;
	char *sceneDataFileName;	
	SceneFileHdr sceneFileHdr;
	SceneFileTileData **sceneFileTileData;	
	bool loadDataFile();
private:
};

typedef list<Scene *> SceneList;

#endif  /* _SCENE_H */
