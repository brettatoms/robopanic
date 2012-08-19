#ifndef _TILE_H
#define _TILE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <list>
#include <ddraw.h>
#include "defines.h"
#include "files.h"

using namespace std;

class BmpLoader;
class BoundingBox;
class SceneCoordinate;

#define NTILE_TYPES (TILES_FILE_ROWS*TILES_FILE_COLUMNS)

class Tile{
public:
	Tile();
	~Tile();	
	bool draw(LPDDSurface surface, int x, int y);
	bool load();
	bool init();
	bool restore();
	void release();
	int type; //  unique identifier				
	static POINT *tileCoords;
	static BmpLoader *tilesBmp; // all tiles share the same bmp
	static int WIDTH;			// all tiles have the same WIDTH and HEIGHT, 32x32
	static int HEIGHT;
	bool getAttribute(int which){ return attributes[which]; };
	void setAttributes(bool attributes[]);
	int row, col;
	BoundingBox *getBoundingBox();
	SceneCoordinate *coord;
protected:	
	bool createSurface();
	bool attributes[NUM_TILE_ATTRIBS];
	static int instances;
};

typedef list<Tile *> TileList;

#endif /* _TILE_H */
