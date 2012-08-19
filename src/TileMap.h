#ifndef _TILE_MAP_H
#define _TILE_MAP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Tile.h"
#include "defines.h"
#include <ddraw.h>
#include "files.h"

//class TileList;
class Tile;
class CBmpFileReader;
class BoundingBox;

enum {TILE_LEVEL_0=0, TILE_LEVEL_1, NUM_TILE_LEVELS};

class TileMap{
public:
	TileMap();
	~TileMap();	
	bool load();	
	bool init(SceneFileTileData **data, int tileRows, int tileCols);
	bool FC draw(LPDDSurface surface, int x, int y);	
	bool FC draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);	
	int width, height;  // in pixels	
	bool restore();
	void release();
	bool loadData(SceneFileTileData **data, int tileRows, int tileCols);
	void unload();
	bool isInitialized();
	Tile *getTileAt(int x, int y);
	//int getTilesInBox(BoundingBox *box, TileList *tile);
	int getTilesInBox(BoundingBox *box, Tile *tile[]);
	Tile *getAdjacentTile(Tile *tile, int d);
	bool FC drawOnTopTiles(LPDDSurface surface);
	Tile *onTopTiles[300]; // pointers into **tiles of the tile with the ontop attrib set
protected:
	int tilesHigh, tilesWide;
	SceneFileTileData **sceneFileTileData;
	bool initialized;
	bool createSurface();	
	//LPDDSurface images[NUM_TILE_LEVELS];	
	LPDDSurface image;
	//Tile *onTopTiles[TILES_FILE_ROWS*TILES_FILE_COLUMNS]; // pointers into **tiles of the tile with the ontop attrib set
	//Tile *onTopTiles[300]; // pointers into **tiles of the tile with the ontop attrib set
	Tile **tiles;	
private:
};

#endif // _TILE_MAP_H