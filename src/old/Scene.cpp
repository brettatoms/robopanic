#include <fstream>

#include "Scene.h"
#include "TileMap.h"
#include "Tile.h"
#include "Entity.h"
#include "SceneCoordinate.h"
#include "Enemy.h"
#include "BoundingBox.h"
#include "Tile.h"
#include "Character.h"
#include "Door.h"
#include "Sound.h"

#include "debug.h"

//using namespace std;

Scene::Scene(char *dataFile){// : tileMapName(dataFile){
	tileMapName = NULL;
	sceneDataFileName = dataFile;
	tileMap = NULL;
	neighbors[NORTH] = neighbors[EAST] = neighbors[SOUTH] = neighbors[WEST] = NULL;	
	doors[NORTH] = doors[EAST] = doors[SOUTH] = doors[WEST] = NULL;	
	width = height = 0;
	initialized = false;
	character = NULL;
	backgroundSoundFileName = NULL;
	completed = false;
}

Scene::~Scene(){
	release();
	if(tileMap) delete tileMap;	
	if(sceneDataFileName) delete sceneDataFileName;
	if(tileMapName) delete tileMapName;
	for(int k=0; k<NUM_DIRECTIONS; k++)
		if(neighbors[k]) delete neighbors[k];	
	for(EntityList::iterator i = entities.begin(); i != entities.end(); i++)
		delete (*i);
	for(EnemyList::iterator j = enemies.begin(); j != enemies.end(); j++)
		delete (*j);

}

/////////////////////////////////////////////////////


// return the number of entities found at scene coord x, y
// and place them in <list>
int Scene::getEntitiesAt(int x, int y, EntityList *elist){		
	EntityList::iterator i;
	int j = 0;
	for(i = entities.begin(); i != entities.end(); i++){
		if((*i)->isCoordInside(x, y)){			
			if(elist != NULL) elist->insert(elist->end(), (*i));
			j++;
			//dprintf(("  inserted entity\n"));
		}
	}
	return j;
}

/////////////////////////////////////////////////////

bool coordBetween(int x1, int x2, int x){
	if(x1 < x2){ if(x > x1 && x < x2) return true;}
	else if(x > x2 && x < x1) return true;
	return false;
}

/////////////////////////////////////////////////////

int Scene::getEntitiesInBox(BoundingBox *box, EntityList *elist){
	
	if(entities.empty()) return 0;	
	int j=0;	
	for(EntityList::iterator i = entities.begin(); i != entities.end(); i++){		
		BoundingBox *entityBox = (*i)->getBaseBoundingBox();
		if(entityBox->overlaps((*box))){
			if(elist != NULL) elist->insert(elist->end(), (*i));
			j++;			
		}
	}
	return j;
}

/////////////////////////////////////////////////////

void Scene::setNeighbors(Scene *north,  Scene *east, Scene *south, Scene *west){
	neighbors[NORTH] = north;
	neighbors[EAST] = east;
	neighbors[SOUTH] = south;
	neighbors[WEST] = west;
}

/////////////////////////////////////////////////////

// undo or delete object setup or created in Scene::load() to 
// conserve memory
void Scene::unload(){
	
	//unload tilemap
	tileMap->unload();

	//unload entities
	EntityList::iterator i;
	for(i = entities.begin(); i != entities.end(); i++)
		(*i)->unload();						
}

/////////////////////////////////////////////////////

int flipBytes(int n){
	return ((n >> 24) & 0x000000FF) | ((n >> 8) & 0x0000FF00) |
		((n << 8) & 0x00FF0000) | ((n << 24) & 0xFF000000);
}

/////////////////////////////////////////////////////

// read in the data from the *.rpd files
// NOTE: the scene file editor was done in java which has a different
// byte ordering than the intel arch so all the bytes have to be flipped
bool Scene::loadDataFile(){	
	FILE *file = fopen(sceneDataFileName, "r");	
	
	fread(&sceneFileHdr, sizeof(sceneFileHdr), 1, file);
	sceneFileHdr.engVersion = flipBytes(sceneFileHdr.engVersion);
	if(sceneFileHdr.engVersion != 70){
		dprintf(("%s: invalid data file version, %d\n", sceneDataFileName, 
			sceneFileHdr.engVersion));
			return false;
	}
	sceneFileHdr.tileRows = flipBytes(sceneFileHdr.tileRows);
	sceneFileHdr.tileCols = flipBytes(sceneFileHdr.tileCols);

	dprintf(("engName: %s\n", sceneFileHdr.engName));
	dprintf(("engVersion == %d\n", sceneFileHdr.engVersion));
	dprintf(("sceneName: %s\n", sceneFileHdr.sceneName));
	dprintf(("sceneDesc: %s\n", sceneFileHdr.sceneDesc));
	dprintf(("tileSet: %s\n", sceneFileHdr.tileSet));
	dprintf(("tileRows: %d\n", sceneFileHdr.tileRows));
	dprintf(("tileCols: %d\n", sceneFileHdr.tileCols));
	
	sceneFileTileData = new SceneFileTileData*[sceneFileHdr.tileRows];
	for(int i=0; i<sceneFileHdr.tileRows; i++){
		sceneFileTileData[i] = new SceneFileTileData[sceneFileHdr.tileCols];		
		for(int j=0; j<sceneFileHdr.tileCols; j++){	
			sceneFileTileData[i][j].type = 0;			// i can get in trouble if i don't 
			sceneFileTileData[i][j].attributes[0] = 0;	// init these before i read into 
			sceneFileTileData[i][j].attributes[1] = 0;	// them, i don't know why?
			sceneFileTileData[i][j].attributes[2] = 0;
			fread(&sceneFileTileData[i][j].type, sizeof(int), 1, file);						
			fread(&sceneFileTileData[i][j].attributes, sizeof(sceneFileTileData[i][j].attributes), 1, file);
			sceneFileTileData[i][j].type = flipBytes(sceneFileTileData[i][j].type);
			dprintf(("  [%d][%d]: %x, %x, %x -----> type: %d\n", i, j, sceneFileTileData[i][j].attributes[0], 
				sceneFileTileData[i][j].attributes[1], sceneFileTileData[i][j].attributes[2], 
				sceneFileTileData[i][j].type));
		}		
	}
	
	return true;
}

/////////////////////////////////////////////////////

extern CSoundManager *soundManager;
bool Scene::playBackgroundSound(){
	if(backgroundSoundFileName == NULL) return false;
	soundManager->play(sound, true);
	return true;
}

/////////////////////////////////////////////////////

// should only initialize once
bool Scene::init(){

	hasEnemies = false;

	dprintf(("entered Scene::init()\n"));
	//init tilemap
	if(!tileMap) tileMap = new TileMap();
		
	if(!loadDataFile()) return false; // load the 
	dprintf(("  data loaded\n"));

	if(!(tileMap->init(sceneFileTileData, sceneFileHdr.tileRows, sceneFileHdr.tileCols))){
		dprintf(("  tilemap NOT initialized\n"));
		return false;
	}
	width = tileMap->width;
	height = tileMap->height;

	dprintf(("  tilemap initialized\n"));

	//init entities
	for(EntityList::iterator i = entities.begin(); i != entities.end(); i++){
		if(!(*i)->init()) {
			dprintf(("  could not init entity\n"));
			return false;
		}
	}

	//init enemies
	for(EnemyList::iterator j = enemies.begin(); j != enemies.end(); j++){
		if(!(*j)->init()) {
			dprintf(("  could not init enemy\n"));
			return false;
		}
	}

	/*
	//init doors
	for(DoorList::iterator k = doors.begin(); k!=doors.end(); k++){
		if(!(*k)->init()){
			dprintf(("  could not init door\n"));
			return false;
		}
	}*/


	// init sounds
	if(backgroundSoundFileName != NULL)
		sound = soundManager->createBuffer(backgroundSoundFileName);

	//if(!entities) entities = new EntityList();
	initialized = true;
	dprintf(("leaving Scene::init()\n"));
	return true;
}

/////////////////////////////////////////////////////

// called each time you enter the scene
bool Scene::load(){
	dprintf(("entered Scene::load()\n"));
	if(!initialized) return false;

	if(!tileMap->load()) return false;

	//load entities
	EntityList::iterator i;
	for(i = entities.begin(); i != entities.end(); i++)
		if(!(*i)->load()) {
			dprintf(("  could not load entity"));
			return false;
		}
	
	dprintf(("leaving Scene::load()\n"));
	return true;
}



/////////////////////////////////////////////////////
/*
bool FC Scene::draw(LPDDSurface surface, int destX, int destY){
	//dprintf(("entered Scene::draw(surface, %d, %d)\n", destX, destY));
	if(!tileMap->draw(surface, destX, destY)) return false;

	// draw all the entities
	for(EntityList::iterator i = entities.begin(); i != entities.end(); i++)
		if(!(*i)->draw(surface, destX+(*i)->getX(), destY+(*i)->getY())) {
			dprintf(("  Scene::draw(): could not draw entity\n"));
		return false;
	}

	// draw all the enemies
	for(EnemyList::iterator j = enemies.begin(); j != enemies.end(); j++){		
		if(!(*j)->draw(surface)) {
			dprintf(("  Scene::draw(): could not draw enemy\n"));
			return false;
		}
	}	


	return true;
}
*/
/////////////////////////////////////////////////////

bool FC Scene::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){

	if(!tileMap->draw(surface, destBox, srcBox)) return false;
	
	for(EnemyList::iterator j = enemies.begin(); j != enemies.end(); j++){
		BoundingBox *eBox = (*j)->getFeetBoundingBox();		
		if(eBox->bottom > srcBox->top && eBox->bottom < srcBox->bottom)		
			if(!(*j)->draw(surface)) return false;
	}

	//tileMap->drawOnTopTiles(surface)
	for(EntityList::iterator i = entities.begin(); i != entities.end(); i++){
		BoundingBox *eBox= (*i)->getBaseBoundingBox();
		//if(eBox->bottom > srcBox->top && eBox->bottom < srcBox->bottom)		
		if(eBox->bottom > srcBox->top)// && eBox->bottom < srcBox->bottom)		
			if(!(*i)->draw(surface)) return false;

	}

	/*
	// draw over enemies but not entities		
	for(int n=0; tileMap->onTopTiles[n] != NULL; n++){		
		Tile *tile = tileMap->onTopTiles[n];
		if(tile->row*Tile::HEIGHT+Tile::HEIGHT > srcBox->top){
			
		//if(tile->row*Tile::HEIGHT+Tile::HEIGHT > srcBox->top){
		//if(tile->row*Tile::HEIGHT > srcBox->top  && 
			//(tile->row*Tile::HEIGHT)+Tile::HEIGHT < srcBox->bottom){
		//if(tile->row*Tile::HEIGHT > srcBox->bottom &&
			//(tile->row*Tile::HEIGHT)+Tile::HEIGHT >srcBox->top){
			dprintf(("---------> draw on top: %d x %d\n", tile->row, tile->col));
			if(!tile->draw(surface, tile->col*Tile::WIDTH, tile->row*Tile::HEIGHT)) 
				return false;
		}
	}*/

	tileMap->drawOnTopTiles(surface);

	/*
	for(DoorList::iterator k = doors.begin(); k != doors.end(); k++){
		BoundingBox *dBox = (*k)->getBaseBoundingBox();
		if(dBox->bottom > srcBox->top && dBox->bottom < srcBox->bottom)
			if(!(*k)->draw(surface)) return false;
	}*/
	

/*


	for(EnemyList::iterator j = enemies.begin(); j != enemies.end(); j++){
		BoundingBox *eBox = (*j)->getFeetBoundingBox();		
		if(eBox->bottom > srcBox->top && eBox->bottom < srcBox->bottom)		
			if(!(*j)->draw(surface)) return false;
	}
*/
	

	return true;
}


/////////////////////////////////////////////////////

//render everything contained in this scene to one surface
bool FC Scene::draw(LPDDSurface surface){
	return draw(surface, 0, 0);	
}

/////////////////////////////////////////////////////

void Scene::giveCharacter(Character **character){
	this->character = (*character);
}

void Scene::removeCharacter(){
	character = NULL;
}

/////////////////////////////////////////////////////

bool Scene::getNeighbor(Direction d, Scene **scene){
	return (*scene = neighbors[d]) != NULL ? true : false;	
}

/////////////////////////////////////////////////////

bool Scene::restore(){
	dprintf(("Entered the Scene::restore\n"));
	//if(!tileMap->restore()) return false;
	if(!initialized)
		return true;
	tileMap->restore();

	for(EntityList::iterator i = entities.begin(); i!=entities.end(); i++)
		(*i)->restore();
	for(EnemyList::iterator j = enemies.begin(); j!=enemies.end(); j++)
		(*j)->restore();	
	
	//for(EntityList::iterator i = entities.begin(); i!=entities.end(); i++)
	//	if((*i)->restore()) return false;
	//for(EnemyList::iterator j = enemies.begin(); j!=enemies.end(); j++)
	//	if((*j)->restore()) return false;	
	return true;
}

void Scene::release(){
	for(EntityList::iterator i = entities.begin(); i!=entities.end(); i++)
		(*i)->release();
	for(EnemyList::iterator j = enemies.begin(); j!=enemies.end(); j++)
		(*j)->release();		
	tileMap->release();
	sound->Release();
	doors[NORTH]->release();
	doors[EAST]->release();
	doors[SOUTH]->release();
	doors[WEST]->release();

}



