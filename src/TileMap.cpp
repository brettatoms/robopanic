#include "TileMap.h"
#include "Tile.h"
#include "BoundingBox.h"
#include "files.h"
#include <string.h>

#include "debug.h"
//#undef DEBUG_ON

extern LPDIRECTDRAW lpDirectDrawObject; //direct draw object
void debugBlt(HRESULT);

TileMap::TileMap(){
	width = 0;
	height = 0;	
	image = NULL;
	initialized = false;
	//for(int i=0; i<Tile::WIDTH*Tile::HEIGHT; i++) onTopTiles[i] = NULL;
	//for(int i=0; i<TILES_FILE_ROWS*TILES_FILE_COLUMNS; i++) onTopTiles[i] = NULL;
	for(int i=0; i<(SCREEN_HEIGHT/Tile::HEIGHT)*(SCREEN_WIDTH/Tile::WIDTH); i++) 
		onTopTiles[i] = NULL;
}

TileMap::~TileMap(){
	release();	
	for(int i=0; i<height; i++)	
		if(tiles[i]) delete[] tiles[i];
	
}

//////////////////////////////////////////////////////////

bool FC TileMap::drawOnTopTiles(LPDDSurface surface){	
	for(int i=0; onTopTiles[i] != NULL; i++){
		Tile *tile = onTopTiles[i];
		if(!tile->draw(surface, tile->col*Tile::WIDTH, tile->row*Tile::HEIGHT)) 
			return false;
	}
	return true;

}

//////////////////////////////////////////////////////////

// NOTE: the tiles are all drawn to a single surface(this->image) in the
//   init function so we don't have to do any iterations in draw()

bool FC TileMap::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){
	//dprintf(("entering TileMap::draw(box, box)\n"));
	HRESULT r;		
	r = surface->Blt(destBox->toRECT(), image, srcBox->toRECT(), DDBLT_WAIT, NULL);			
	//dprintf(("leaving TileMap::draw(box box)\n"));
	return ((r == DD_OK) ? true : false);
}

//////////////////////////////////////////////////////////

bool FC TileMap::draw(LPDDSurface surface, int destX, int destY){
 	//dprintf(("entered TileMap::draw()\n"));
	HRESULT r;
	
	RECT destRect;	
	destRect.bottom = destY > 0 ? SCREEN_HEIGHT : SCREEN_HEIGHT + destY;
	destRect.right = destX > 0 ? SCREEN_WIDTH: SCREEN_WIDTH + destX;
	destRect.top = destY;
	destRect.left = destX;	

	RECT srcRect;
	srcRect.top = 0;
	srcRect.left = 0;
	srcRect.bottom = destY > 0 ? height-destY : height;
	srcRect.right = destX > 0 ? width-destX : width;
	
	r = surface->Blt(&destRect, image, &srcRect, DDBLT_WAIT, NULL);		

	//dprintf(("leaving TileMap::draw()\n"));
	return ((r == DD_OK) ? true : false);
}

//////////////////////////////////////////////////////////

// call each time the TileMap is being displayed
bool TileMap::load(){
	dprintf(("entered TileMap::load()\n"));	
	if(!initialized) return false;
	return true;
}

//////////////////////////////////////////////////////////

// this is ugly but avoids round off error for now
Tile *TileMap::getTileAt(int x, int y){	
	if(x < 0) x = 0;
	if(y<0) y=0;	
	int col= x/Tile::WIDTH, row = y/Tile::HEIGHT;
	//dprintf(("getTileAt: %d, %d ------ %d, %d\n", x, y, row, col));
	//if(col >
	return &tiles[row][col];	
}

//////////////////////////////////////////////////////////

// return each tile which overlaps with <box> and put the tiles in <tileList>, 
// first two tiles in the list are the tiles which intersect the top of the 
// <box>, the next and last two are the tiles which intersect the bottom
// of the <box>, there may be duplicates in the list
// 0 1
// 2 3
int TileMap::getTilesInBox(BoundingBox *box, Tile *tileReturn[]){
	tileReturn[0] = getTileAt(box->left, box->top);
	tileReturn[1] = getTileAt(box->right, box->top);
	tileReturn[2] = getTileAt(box->left, box->bottom);
	tileReturn[3] = getTileAt(box->right, box->bottom);
	return 4;	
}

//////////////////////////////////////////////////////////

// return the tile to the direction <d> of <tile>
Tile *TileMap::getAdjacentTile(Tile *tile, int d){
	switch(d){
	case NORTH:
		if(tile->row != 0) return &(this->tiles[tile->row-1][tile->col]);
	case SOUTH: 
		if(tile->row != tilesHigh-1) return &(this->tiles[tile->row+1][tile->col]);
	case EAST: 
		if(tile->col !=0) return &(this->tiles[tile->row][tile->col-1]);
	case WEST: 
		if(tile->col != tilesWide-1) return &(this->tiles[tile->row][tile->col+1]);	
	default: return NULL;
	}

	return NULL;	
}

//////////////////////////////////////////////////////////

// read the tilemap data from <filename> and draw each tile to
// one offscreen surface for blitting later
// called once/instantiation
bool TileMap::init(SceneFileTileData **data, int tileRows, int tileCols){

	tilesHigh = tileRows; tilesWide = tileCols;
	height = tileRows*Tile::HEIGHT;
	width = tileCols*Tile::WIDTH;	
	dprintf(("%d x %d\n", tileRows, tileCols));

	if(!createSurface()) return false; // create the surface to layout the tiles	

	// read in each tile and draw it to it's x, y location in this->image
	
	dprintf(("  reading tiles: %d x %d\n", width, height));	
	tiles = new Tile*[tileRows];
	
	//trying this out
	sceneFileTileData = data;
	loadData(data, tileRows, tileCols);
	
	
	initialized = true;
	dprintf(("leaving TileMap::init() -- width: %d, height %d\n", width, height));
	return true;
}


bool TileMap::loadData(SceneFileTileData **data, int tileRows, int tileCols){
	int onTopIndex = 0;
	for(int i=0; i<tileRows; i++){		// i = height
		tiles[i] = new Tile[tileCols];	// j = width				
		for(int j=0; j<tileCols; j++){
			//dprintf(("[%d][%d]: %d\n", i, j, data[i][j].type));
			if(data[i][j].type == -1) // -1 and the last tile type are the blank tile
				tiles[i][j].type = TILES_FILE_ROWS*TILES_FILE_COLUMNS;
			else tiles[i][j].type = data[i][j].type;
			if(!tiles[i][j].init()) return false;			
			if(!tiles[i][j].draw(image, j*Tile::WIDTH, i*Tile::HEIGHT)){
				//dprintf(("  could not draw tile[%d][%d](%d): %d x %d\n", i, j, 
					//tiles[i][j].type,j*Tile::WIDTH, i*Tile::HEIGHT));
				return false;
			}
			tiles[i][j].row = i;  // set the tiles row and col 
			tiles[i][j].col = j;  
			tiles[i][j].setAttributes(data[i][j].attributes); // set the attributes
			if(tiles[i][j].getAttribute(TILE_ONTOP))
				onTopTiles[onTopIndex++] = &tiles[i][j];
				
		}
	}
	return true;
}
//////////////////////////////////////////////////////////

void TileMap::unload(){
	
}

//////////////////////////////////////////////////////////

bool TileMap::createSurface(){
	DDSurfaceDesc ddsd;	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);							 //required field
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;// | DDSD_CKSRCBLT;		
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;		 //offscreen surface
	ddsd.dwHeight = height;
	ddsd.dwWidth = width;		
	//ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = TRANSPARENT_COLOR;
	//ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = TRANSPARENT_COLOR;
	HRESULT result;	
	result = lpDirectDrawObject->CreateSurface(&ddsd, &image, NULL);	
	return (result == DD_OK) ? true : false;	
}

//////////////////////////////////////////////////////////

bool TileMap::restore(){	
	dprintf(("Entered the TileMap::restore\n"));

	dprintf(("ABOUT TO RESTORE THE tilesBmp"));
	tiles[0][0].restore();
	dprintf(("ABOUT TO RESTORE THE image"));
	if(image->IsLost() == DDERR_SURFACELOST)
		image->Restore();
	loadData(sceneFileTileData, tilesHigh, tilesWide);
	//draw(image,0,0);

	dprintf(("Leaving the TileMap::restore\n"));
	return true;
}

//////////////////////////////////////////////////////////

void TileMap::release(){
	for(int i=0; i<height; i++)
		for(int j=0; j<height; j++)
			tiles[i][j].release();
	image->Release();
}

