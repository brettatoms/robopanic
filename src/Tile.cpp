// Tile.cpp: implementation of the Tile class.
//
//////////////////////////////////////////////////////////////////////

#include "Tile.h"
#include "BmpLoader.h"
#include "BoundingBox.h"
#include "SceneCoordinate.h"
#include "debug.h"

extern LPDirectDraw lpDirectDrawObject;
extern LPDDSurface lpPrimary;

//initialize static members
int Tile::instances = 0;
int Tile::WIDTH = 32;
int Tile::HEIGHT = 32;
BmpLoader *Tile::tilesBmp = NULL;
POINT *Tile::tileCoords = NULL; // contains the coordinates for the tiles
								// on the contact sheet bmp

/////////////////// constructor/destructor ///////////////////////

Tile::Tile(){
	type = -1; // no tile defined
	coord = new SceneCoordinate();
	row = col = -1;
	if(tileCoords == NULL){ // init the tile coords if they haven't been
		tileCoords = new POINT[TILES_FILE_ROWS*TILES_FILE_COLUMNS];
		for(int i=0; i<TILES_FILE_ROWS; i++){
			for(int j=0; j<TILES_FILE_COLUMNS; j++){
				int index = (i*TILES_FILE_COLUMNS)+j;
				tileCoords[index].x = ((WIDTH+1)*j)+1; 
				tileCoords[index].y = ((HEIGHT+1)*i)+1;
				if(index == 19) {
					dprintf(("*************** %d, %d ***********\n", tileCoords[index].x,	
					tileCoords[index].y));
				}
			}
		}
	}// end if tileCoords == NULL
}

Tile::~Tile(){
	if(instances == 0)	delete[] tileCoords;		
	release();
}

////////////////////////////////////////////////////

BoundingBox *Tile::getBoundingBox(){
	return new BoundingBox(row*Tile::HEIGHT, col*Tile::WIDTH, 
		(row*Tile::HEIGHT)+Tile::HEIGHT, (col*Tile::WIDTH)+Tile::WIDTH);	
}

////////////////////////////////////////////////////


//member functions

bool Tile::restore(){
	dprintf(("Entering the Tile::restore\n"));
	if(tilesBmp->surfaceLost() == true){
		dprintf(("The tilesBmp Surface is Lost\n"));		
		tilesBmp->restore();
		dprintf(("Restored the tilesBmp SUrface\n"));
	}
	dprintf(("Leaving the Tile::restore\n"));
	return false;
}

void Tile::release(){
	if(instances == 0)
		tilesBmp->release();	
}

void Tile::setAttributes(bool attrib[]){
	for(int i=0; i<NUM_TILE_ATTRIBS; i++)
		attributes[i] = attrib[i];
}

void clearSurface(LPDDSurface surface);


// loaded each time the tile is used
bool Tile::load(){
	return true;
}

// init an image according to what Tile.type is set to
// called only once for each tile
bool Tile::init(){	
	if(type < 0) return false;  // no tile defined
	if(Tile::tilesBmp == NULL){  // have tiles share one bmp file		
		Tile::tilesBmp = new BmpLoader();		
		if(!Tile::tilesBmp->load(TILES_FILE_NAME)){
			dprintf(("  could not load file: %s\n", TILES_FILE_NAME));
			return false;
		}else dprintf((" tile.bmp: %d, %d\n", tilesBmp->getWidth(),
			tilesBmp->getHeight()));
	}
	instances ++; // if == 0 then we can release the tilesBmp
	return true;	
}

// draw this->image to surface at x,y
bool Tile::draw(LPDDSurface surface, int destX, int destY){	
	return tilesBmp->drawRectTo(destX, destY, surface, tileCoords[type].x, tileCoords[type].y, WIDTH, HEIGHT);	
}
