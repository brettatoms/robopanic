#include "World.h"

#include "Scene.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include "Scene6.h"
#include "Scene7.h"
#include "Tile.h"
#include "TileMap.h"
#include "files.h"
#include "debug.h"
#include "character.h"
#include "SceneCoordinate.h"
#include "BoundingBox.h"
#include "Tile.h"
#include "Bullet.h"
#include "Enemy.h"
	
extern LPDDSurface lpPrimary;

Scene1 *scene1 = NULL;	
Scene2 *scene2 = NULL;
Scene3 *scene3 = NULL;
Scene4 *scene4 = NULL;
Scene5 *scene5 = NULL;
Scene6 *scene6 = NULL;
Scene7 *scene7 = NULL;

World::World(){
	character = NULL;
	currScene = NULL;	

	sceneHasChanged = false;
	characterDirection = SOUTH;
	lastBulletTime=GetTickCount();
	bulletCounter=330;
}

////////////////////////////////////////////////////////

//initialize all of the scenes in our world, set each scene's neighbors
bool World::initialize(){
	dprintf(("entered World::initialize()\n"));	
	srand(GetTickCount());

	scene1 = new Scene1(SceneDataFileNames[0]);	
	scene2 = new Scene2(SceneDataFileNames[1]);
	scene3 = new Scene3(SceneDataFileNames[2]);
	scene4 = new Scene4(SceneDataFileNames[3]);
	scene5 = new Scene5(SceneDataFileNames[4]);
	scene6 = new Scene6(SceneDataFileNames[5]);
	scene7 = new Scene7(SceneDataFileNames[6]);
	
	// tell each scene what other scenes are adjacent to it
	//                   N			E		S		W      	

	scene1->setNeighbors(scene2,	scene4,	NULL,	NULL);	
	scene2->setNeighbors(scene6,	scene3,	scene1, NULL);
	scene3->setNeighbors(scene5,	NULL,	NULL,	scene2);
	scene4->setNeighbors(NULL,		NULL,	NULL,	scene1);
	scene5->setNeighbors(NULL,		NULL,	scene3, scene6);
	scene6->setNeighbors(NULL,		scene5, scene2, scene7);
	scene7->setNeighbors(NULL,		scene6, NULL,	NULL);

	currScene = scene1;  // start locations
	prevScene = currScene;
	prevDirection = NORTH; // arbitrary
	
	character = new	Character(CHAR_FILE_NAME, CHAR_FRAMES, CHAR_WIDTH, CHAR_HEIGHT,
							  CHAR_ROWS, CHAR_COLUMNS);	
	//character->setPosition(200, 200);
	character->setPosition(75, 75);
	character->setSpeed(6,6);
	character->setAnimationSpeed(100);	

	currScene->giveCharacter(&character);

	dprintf(("  about to init and load currScene\n"));	
	if(!currScene->init()) return false;
	if(!currScene->load()) return false;	

	dprintf(("leaving World::initialize()\n"));	
	return true;
}


World::~World(){
	release();
	if(scene1) delete scene1;
	if(scene2) delete scene2;
	if(scene3) delete scene3;
	if(scene4) delete scene4;
	if(scene5) delete scene5;
	if(scene6) delete scene6;
	if(scene7) delete scene7;	
	if(character) delete character;
	if(currScene) delete currScene;
}

////////////////////////////////////////////////////////

int World::getEntitiesAt(int x, int y, EntityList *elist){
	return currScene->getEntitiesAt(x, y, elist);
}

////////////////////////////////////////////////////////

void World::reactToAction(int actionType){
	switch(actionType){
	case ACTION_PUSH:
		pushActionCallback();
		break;
	case ACTION_PICKUP:
		pickupActionCallback();
		break;
	}
}

////////////////////////////////////////////////////////

void World::pickupActionCallback(){
	EntityList *elist = new EntityList();
	BoundingBox *box = character->getFeetBoundingBox();
	int nEntities = currScene->getEntitiesInBox(box, elist);	
	for(EntityList::iterator i=elist->begin(); i!=elist->end(); i++){
		ActionList actionList = (*i)->getActionList();
		for(ActionList::iterator j = actionList.begin(); j!=actionList.end(); j++){			
			if((*j)->type == ACTION_PICKUP && (*i)->getAttribute(ENTITY_PICKUPABLE)){
				(*j)->react();				
			}
		}
	}			
}

//ActionCallback World::pushActionCallback(){
void World::pushActionCallback(){
	BoundingBox *box = character->getBoundingBox();		
	EntityList *elist = new EntityList();
	int nEntities = currScene->getEntitiesInBox(box, elist);	
	for(EntityList::iterator i=elist->begin(); i!=elist->end(); i++){
		ActionList actionList = (*i)->getActionList();
		for(ActionList::iterator j = actionList.begin(); j!=actionList.end(); j++){			
			if((*j)->type == ACTION_PUSH){				
				(*j)->react();				
			}
		}
	}			
}

////////////////////////////////////////////////////////

// change currScene, prevScene and prevDirection
bool World::changeScene(Direction d){
	/*
	currScene->unload();
	if(!currScene->getNeighbor(d, &currScene)) return false;
	if(!currScene->isInitialized()) currScene->init();
	if(!currScene->load()) return false;
	*/
	
	Scene *temp = currScene;
	currScene->unload();
	if(!currScene->getNeighbor(d, &temp)) return false;
	temp->giveCharacter(&character);
	if(!temp->isInitialized()) temp->init();
	if(!temp->load()) return false;
		
	switch(d){
		case NORTH: prevDirection = SOUTH; break;
		case EAST: prevDirection = WEST; break;
		case SOUTH: prevDirection = NORTH; break;
		case WEST: prevDirection = EAST; break;
	}
		
	if(prevScene != temp) temp->addEnemy();	

	prevScene = currScene;
	currScene = temp;
	prevScene->removeCharacter();				// take the character from the previous 
	//currScene->giveCharacter(character);		// scene and give it the the new one

	for(BulletList::iterator i = bulletList.begin(); i != bulletList.end(); i++)
		(*i)->setBulletState(DEAD);

	sceneHasChanged = true;
	return true;
}

////////////////////////////////////////////////////////

// draw the new scene scrolling from the previous
// return true when the scrolling is complete
bool FC World::scrollInNewScene(LPDDSurface surface){
	//dprintf(("**************** entered World::scrollInNewScene()\n"));
	static int newX = 0, newY = 0;
	Scene *temp = NULL;
	currScene->getNeighbor(prevDirection, &temp);
	if(temp == NULL){ // no scene to scroll in so were done
		newX = newY = 0;
		sceneHasChanged = false;
		return true;		
	}

	unsigned int scale = 4; // the higher the # the faster it scrolls
	bool done = false;
	if(prevDirection == SOUTH){		// moving up				
		prevScene->tileMap->draw(surface, 0, newY);
		currScene->tileMap->draw(surface, 0, newY-SCREEN_HEIGHT);
		newY += scale;
		if(newY >= SCREEN_HEIGHT) done = true;					
	}
	else if(prevDirection == NORTH){    // moving down
		prevScene->tileMap->draw(surface, 0, newY);
		currScene->tileMap->draw(surface, 0, SCREEN_HEIGHT+newY);
		newY -= scale;
		if(newY <= 1-SCREEN_HEIGHT) done = true;
	}
	else if(prevDirection == EAST){		// moving left
		prevScene->tileMap->draw(surface, newX, 0);
		currScene->tileMap->draw(surface, newX-SCREEN_WIDTH, 0);				
		newX += scale;
		if(newX >= SCREEN_WIDTH) done = true;
	}
	else if(prevDirection == WEST){     // moving right				
		prevScene->tileMap->draw(surface, newX, 0);
		currScene->tileMap->draw(surface, SCREEN_WIDTH+newX, 0);
		newX -= scale;
		if(newX <= 1-SCREEN_WIDTH) done = true;
	}
	
	//dprintf(("leaving World::scrollInNewScene()\n"));
	if(done){
		newX = newY = 0;
		sceneHasChanged = false;
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////

void World::setCharacterDirection(Direction direction){
	characterDirection = direction;
}

Direction World::getCharacterDirection(){
	return characterDirection;
}

////////////////////////////////////////////////////////
void World::advanceEnemies(){
	int x,y;
	int randNum;
	int nTiles = 0;
	int nEntities = 0;
	bool tilePassable = true;
	Tile *tileArray[4];
	EntityList *elist = new EntityList;
	BoundingBox *enemyFeetBox = NULL;
	BoundingBox *ebox = NULL;
	Direction direction;
	
	character->getPosition(x,y);

	for(EnemyList::iterator a = currScene->enemies.begin(); a != currScene->enemies.end(); a++){
		if((*a)->getEnemyState()==DEAD){
			currScene->enemies.remove(*a);
			a = currScene->enemies.begin();
		}
	}

	//dprintf(("Entering World::advanceEnemies\n"));
	for(EnemyList::iterator j = currScene->enemies.begin(); j != currScene->enemies.end(); j++){
		//move the enemy
		(*j)->move();
		(*j)->checkOnScreen();
		if((*j)->getAIState() == SHOOT){
			dprintf(("ABOUT TO CREATE AN ENEMY BULLET\n"));
			createEnemyBullet((*j));
		}

		//check to see if the character is in the enemy's line of sight
		//if the character is in the LOS, state will change to attack
		(*j)->giveCharacterCoords(x,y);
		enemyFeetBox = (*j)->getFeetBoundingBox();
		dprintf(("GOT THE FEET BOUNDING BOX\n"));
		nTiles = currScene->tileMap->getTilesInBox(enemyFeetBox, tileArray);
		nEntities = currScene->getEntitiesInBox(enemyFeetBox, elist);
		dprintf(("GOT THE nEntities\n"));
		if(nEntities > 0) ebox = elist->front()->getBaseBoundingBox();
		direction = (*j)->getCurrentDirection();
		switch(direction){
		case NORTH:
			if(nTiles > 0)
				tilePassable = tileArray[0]->getAttribute(TILE_WALKTHRU) & 
				tileArray[1]->getAttribute(TILE_WALKTHRU);	

			if(nEntities != 0 && tilePassable && (enemyFeetBox->top-ebox->bottom <= (*j)->getSpeed())
				&& !elist->front()->getAttribute(ENTITY_WALKTHRU)){	
				randNum=rand()%3;
				if(randNum == 0)
					(*j)->setCurrentAnimation(WEST);
				else if (randNum == 1)
					(*j)->setCurrentAnimation(EAST);
				else 
					(*j)->setCurrentAnimation(SOUTH);
				(*j)->moveBack(SOUTH);
			}
			else if(!tilePassable){ 
				randNum=rand()%2;
				if(randNum == 0)
					(*j)->setCurrentAnimation(WEST);
				else
					(*j)->setCurrentAnimation(EAST);
				(*j)->moveBack(SOUTH);
			}
			break;
		case SOUTH:
			if(nTiles > 0)	
				tilePassable = tileArray[2]->getAttribute(TILE_WALKTHRU) & 
				tileArray[3]->getAttribute(TILE_WALKTHRU);	
			if(nEntities != 0 && tilePassable && (enemyFeetBox->top-ebox->bottom <= (*j)->getSpeed())
				&& !elist->front()->getAttribute(ENTITY_WALKTHRU)){	
				randNum=rand()%3;
				if(randNum == 0)
					(*j)->setCurrentAnimation(WEST);
				else if (randNum == 1)
					(*j)->setCurrentAnimation(EAST);
				else 
					(*j)->setCurrentAnimation(NORTH);
				(*j)->moveBack(NORTH);
			}
			else if(!tilePassable){
				randNum=rand()%2;
				if(randNum == 0)
					(*j)->setCurrentAnimation(WEST);
				else
					(*j)->setCurrentAnimation(EAST);
				(*j)->moveBack(NORTH);
			}
			break;
		case EAST:
			if(nTiles > 0)	
				tilePassable = tileArray[1]->getAttribute(TILE_WALKTHRU) & 
				tileArray[3]->getAttribute(TILE_WALKTHRU);
			if(nEntities != 0 && tilePassable && (enemyFeetBox->left-ebox->right <= (*j)->getSpeed())
				&& !elist->front()->getAttribute(ENTITY_WALKTHRU)){	
				randNum=rand()%3;
				if(randNum == 0)
					(*j)->setCurrentAnimation(NORTH);
				else if (randNum == 1)
					(*j)->setCurrentAnimation(SOUTH);
				else 
					(*j)->setCurrentAnimation(WEST);
				(*j)->moveBack(WEST);
			}
			else if(!tilePassable){ 
				randNum=rand()%2;
				if(randNum == 0)
					(*j)->setCurrentAnimation(SOUTH);
				else
					(*j)->setCurrentAnimation(NORTH);	
				(*j)->moveBack(WEST);
			}
			break;
		case WEST:
				if(nTiles > 0)	
				tilePassable = tileArray[0]->getAttribute(TILE_WALKTHRU) & 
				tileArray[2]->getAttribute(TILE_WALKTHRU);
				if(nEntities != 0 && tilePassable && (enemyFeetBox->left-ebox->right <= (*j)->getSpeed())
					&& !elist->front()->getAttribute(ENTITY_WALKTHRU)){		
					randNum=rand()%3;
					if(randNum == 0)
						(*j)->setCurrentAnimation(NORTH);
					else if (randNum == 1)
						(*j)->setCurrentAnimation(SOUTH);
					else 
						(*j)->setCurrentAnimation(EAST);
					(*j)->moveBack(EAST);
				}
				else if(!tilePassable){
					randNum=rand()%2;
					if(randNum == 0)
						(*j)->setCurrentAnimation(SOUTH);
					else
						(*j)->setCurrentAnimation(NORTH);	
					(*j)->moveBack(EAST);
				}
			break;
		}
		if(!(*j)->animate()) {
			dprintf(("  Scene::animate(): not working\n"));
		}
	}

	

	//dprintf(("Leaving World::advanceEnemies\n"));
}

// return true or false if it is ok for enemy to move to the new x and y
//bool World::approveEnityMove(Entity *entity, int newX, int newY, Direction d){
bool World::approveEntityMove(Entity *entity, int newX, int newY){	
	int oldX = entity->getX(), oldY = entity->getY();
	entity->setSceneCoord(newX, newY);
	BoundingBox *newBox = entity->getBaseBoundingBox();	

	Tile *tileArray[4];
	int nTiles = 0;
	nTiles = currScene->tileMap->getTilesInBox(newBox, tileArray);	
	for(int i=0; i<4; i++)
		if(!tileArray[i]->getAttribute(TILE_WALKTHRU)){
			entity->setSceneCoord(oldX, oldY);
			return false;
		}
	return true;

}

////////////////////////////////////////////////////////

// advance the character along his path taking care of collision detection 
// with entities an non passable tiles
// how? -- we move the character first and then we determine if he is somewhere
// he isn't supposed to be, if he is then we move him back to the boundary
// of the object he is violating, the boundary depends on the directory
// the character is moving 
void World::advanceCharacter(){	
	EntityList *elist = new EntityList;
	int nEntities = 0;
	bool tilePassable = true;	
	
	// move must be called first so that the coordinates and bounding boxes
	// will contain the correct values
	character->move(characterDirection);  
	SceneCoordinate *center = character->getCenterPoint();
	SceneCoordinate *charPos= character->getPosition();	
	BoundingBox *charFeetBox = character->getFeetBoundingBox();

	// do this twice in case the character picked the entity up
	nEntities = currScene->getEntitiesInBox(charFeetBox, elist); // check for entities	
	reactToAction(ACTION_PICKUP);
	nEntities = currScene->getEntitiesInBox(charFeetBox, elist); // check for entities	

	BoundingBox *ebox = NULL;				// bounding box if each entitity in elist	
	Tile *tileArray[4];
	int nTiles = 0;
	// only get the bounding box if the character is standing on the screen
	if(charFeetBox->bottom < SCREEN_HEIGHT && charFeetBox->top > 0 &&
		charFeetBox->left < SCREEN_WIDTH && charFeetBox->right > 0)
		nTiles = currScene->tileMap->getTilesInBox(charFeetBox, tileArray);	
	
	if(nEntities > 0) ebox = elist->front()->getBaseBoundingBox();

	// direction specific for moving the character
	switch(characterDirection){		
	case NORTH:				
		if(character->getCurrentAnimation() != NORTH) character->setCurrentAnimation(NORTH);		
		if(nTiles > 0) // get the passability of the tile
			tilePassable = tileArray[0]->getAttribute(TILE_WALKTHRU) & 
				tileArray[1]->getAttribute(TILE_WALKTHRU);		

		if(tilePassable){ // the tile is passable
			if(nEntities == 0){ // no entities to worry about
				if(center->y<=0){	// change scene if reach edge
					character->setPosition(charPos->x, charPos->y+SCREEN_HEIGHT);
					changeScene(NORTH);
				}
			}
			else if(nEntities > 0){	// collided with one or more entities
				Entity *e = elist->front();
				SceneCoordinate *eCoord = e->getSceneCoord();
				int newX = eCoord->x, newY = eCoord->y-character->getYSpeed();
				if(e->getAttribute(ENTITY_MOVEABLE) && approveEntityMove(e, newX, newY))
					e->setSceneCoord(newX, newY);
				else if(!elist->front()->getAttribute(ENTITY_WALKTHRU))
					character->setPosition(charPos->x, 
						charPos->y+(ebox->bottom-charFeetBox->top)+1);
			}
		}
		else if(!tilePassable){  // the tile is not passable
			character->setPosition(charPos->x, charPos->y +
				(tileArray[0]->getBoundingBox()->getBottom() - charFeetBox->getTop())+1);			
		}		  
		break;
		
	case EAST:
		if(character->getCurrentAnimation() != EAST) character->setCurrentAnimation(EAST);		
		if(nTiles > 0)	
			tilePassable = tileArray[1]->getAttribute(TILE_WALKTHRU) & 
				tileArray[3]->getAttribute(TILE_WALKTHRU);

		if(tilePassable){
			if(nEntities == 0){
				if(center->x >= SCREEN_WIDTH){ // change scene if reach edge				
					character->setPosition(charPos->x-SCREEN_WIDTH, charPos->y);
					changeScene(EAST);
				}
			}
			else if(nEntities > 0){								
				Entity *e = elist->front();
				SceneCoordinate *eCoord = e->getSceneCoord();					
				int newX = eCoord->x+character->getXSpeed(), newY = eCoord->y;
				if(e->getAttribute(ENTITY_MOVEABLE) && approveEntityMove(e, newX, newY)){
					e->setSceneCoord(newX, newY);						
				}
				else if(!elist->front()->getAttribute(ENTITY_WALKTHRU)){
					character->setPosition(ebox->left-character->width()-1, charPos->y);						
				}				
			}
		}
		else if(!tilePassable && ((charPos->x+character->width())<(SCREEN_WIDTH-15))){
			BoundingBox *tileBox = tileArray[3]->getBoundingBox();
			character->setPosition(charPos->x-(charFeetBox->right-tileBox->left)-1, 
				charPos->y);			
		}
		break;

	case SOUTH:
		if(character->getCurrentAnimation() != SOUTH) character->setCurrentAnimation(SOUTH);
		if(nTiles > 0)	
			tilePassable = tileArray[2]->getAttribute(TILE_WALKTHRU) & 
				tileArray[3]->getAttribute(TILE_WALKTHRU);		

		if(tilePassable){ 
			if(nEntities == 0){				
				if(center->y >= SCREEN_HEIGHT){ // change scene if reach edge							
					character->setPosition(charPos->x,charPos->y-SCREEN_HEIGHT);
					changeScene(SOUTH);
				}
			}
			else if(nEntities > 0){		
				Entity *e = elist->front();
				SceneCoordinate *eCoord = e->getSceneCoord();
				int newX = eCoord->x, newY = eCoord->y+character->getYSpeed();
				if(e->getAttribute(ENTITY_MOVEABLE) && approveEntityMove(e, newX, newY)){ 
					e->setSceneCoord(newX, newY);						
				}
				else if(!elist->front()->getAttribute(ENTITY_WALKTHRU)){
					character->setPosition(charPos->x, 
						charPos->y-(charFeetBox->bottom-ebox->getTop())-1);
				}				
			}
		}
		else if(!tilePassable){
			BoundingBox *tileBox = tileArray[3]->getBoundingBox();			
			character->setPosition(charPos->x, 
				charPos->y-(charFeetBox->bottom-tileBox->top)-1);
		}
		break;

	case WEST:		
		if(character->getCurrentAnimation() != WEST) character->setCurrentAnimation(WEST);
		if(nTiles > 0)	
			tilePassable = tileArray[0]->getAttribute(TILE_WALKTHRU) & 
				tileArray[2]->getAttribute(TILE_WALKTHRU);
		if(tilePassable){
			if(nEntities == 0){
				if(center->x<=0){ // change scene if reach edge
					character->setPosition(charPos->x+SCREEN_WIDTH, charPos->y);
					changeScene(WEST);
				}						
			}
			else if(nEntities > 0){						
				Entity *e = elist->front();
				SceneCoordinate *eCoord = e->getSceneCoord();										
				int newX = eCoord->x-character->getXSpeed(), newY = eCoord->y;					
				if(e->getAttribute(ENTITY_MOVEABLE) && approveEntityMove(e, newX, newY)){
					e->setSceneCoord(newX, newY);						
				}
				else if(!elist->front()->getAttribute(ENTITY_WALKTHRU)){
					character->setPosition(ebox->right+1, charPos->y);						
				}			
			}
		}
		else if(!tilePassable){
			BoundingBox *tileBox = tileArray[2]->getBoundingBox();
			character->setPosition(charPos->x-(charFeetBox->left-tileBox->right), 
				charPos->y);
		}	
		break;
	}

	delete center;
	delete charPos;
	delete charFeetBox;
	delete ebox;
	delete elist;
}// end advanceCharacter()


////////////////////////////////////////////////////////

void World::advanceBullets(){
	BoundingBox *box;
	

	//check to see if the bullet has hit an inpassable tile
	bool tilePassable = true;
	int nTiles = 0;
	Tile *tileArray[4];
	

	for(BulletList::iterator a = bulletList.begin(); a != bulletList.end(); a++){
		tilePassable=true;
		box=(*a)->getBoundingBox();
		nTiles = currScene->tileMap->getTilesInBox(box, tileArray);	
		if(tileArray[0] != NULL && tileArray[1] != NULL &&
			tileArray[2] != NULL && tileArray[3] != NULL){
				tilePassable = tileArray[0]->getAttribute(TILE_WALKTHRU) ||
					   tileArray[1]->getAttribute(TILE_WALKTHRU) ||
				       tileArray[2]->getAttribute(TILE_WALKTHRU) ||
				       tileArray[3]->getAttribute(TILE_WALKTHRU);
		}
		if(!tilePassable)
			(*a)->setBulletState(DEAD);
	}
	

	for(BulletList::iterator b = enemyBulletList.begin(); b != enemyBulletList.end(); b++){
		tilePassable=true;
		box=(*b)->getBoundingBox();
		nTiles = currScene->tileMap->getTilesInBox(box, tileArray);	
		tilePassable = tileArray[0]->getAttribute(TILE_WALKTHRU) ||
				       tileArray[1]->getAttribute(TILE_WALKTHRU) ||
				       tileArray[2]->getAttribute(TILE_WALKTHRU) ||
				       tileArray[3]->getAttribute(TILE_WALKTHRU);
		if(!tilePassable)
			(*b)->setBulletState(DEAD);
	}



	//check to see if bullets have hit any enemies
	for(BulletList::iterator h = bulletList.begin(); h != bulletList.end(); h++){
		if( (*h)->getBulletState()==DEAD){	
				bulletList.remove(*h);
				// reassign i in case we removed the first bullet,
				// this makes it so we have to re-search the entire list every
				// time we remove a bullet
				h = bulletList.begin();  
		}
		else{
			box=(*h)->getBoundingBox();
			for(EnemyList::iterator j = currScene->enemies.begin(); j != currScene->enemies.end(); j++){
				if(box->overlaps((*(*j)->getBoundingBox()))){
					(*j)->changeHealth(10);
					(*h)->setBulletState(DEAD);
					//if((*j)->getEnemyState()==DEAD){
					//	currScene->enemies.remove(*j);
					//	j = currScene->enemies.begin();
					//}
				}
			}
		}		
	}

	for(BulletList::iterator k = enemyBulletList.begin(); k != enemyBulletList.end(); k++){
		if((*k)->getBulletState()==DEAD){
			enemyBulletList.remove(*k);
			k = enemyBulletList.begin();
		}
		else{
			box=(*k)->getBoundingBox();
			if(box->overlaps(*(character->getBoundingBox()))){
				(*k)->setBulletState(DEAD);
				character->changeHealth(-20);
				character->setCharacterState(HIT);
			}
		}
	}

	for(BulletList::iterator i = bulletList.begin(); i != bulletList.end(); i++){
		(*i)->move();
	}

	for(BulletList::iterator j = enemyBulletList.begin(); j != enemyBulletList.end(); j++){
		(*j)->move();
	}
}//end advanceBullets

////////////////////////////////////////////////////////

void World::checkCharacterCollision(){
	SceneCoordinate *center = character->getCenterPoint();
	BoundingBox *box = character->getFeetBoundingBox();;
	Direction cDirection = character->getCurrentDirection();
	SceneCoordinate *enemyCenter;
	int distanceX, distanceY;

	for(EnemyList::iterator j = currScene->enemies.begin(); j!= currScene->enemies.end(); j++){
		if(box->overlaps((*(*j)->getFeetBoundingBox()))){
			if(character->getCharacterState()==ALIVE){
				character->changeHealth(-20);
				character->setCharacterState(HIT);
				enemyCenter=(*j)->getCenterPoint();
				distanceY=( ((*j)->getHeight() + character->height()) / 2);
				distanceX=( ((*j)->getWidth() + character->width()) / 2);
			
				switch((*j)->getCurrentDirection()){
				case EAST:
					if(cDirection == WEST || cDirection == EAST){
						if(center->x < enemyCenter->x)
							character->moveDirection(WEST);
						else
							character->moveDirection(EAST);
					}

					else if (cDirection == NORTH || cDirection == SOUTH){
						if( (center->x > enemyCenter->x) && 
						(enemyCenter->y+distanceY > center->y) &&
						(enemyCenter->y-distanceY < center->y) )
							character->moveDirection(EAST);
						else{
							if(cDirection == NORTH)
								character->moveDirection(SOUTH);
							else 
								character->moveDirection(NORTH);
						}
					}
					break;
				case WEST:
					if(cDirection == WEST || cDirection == EAST){
						if(center->x > enemyCenter->x)
							character->moveDirection(EAST);
						else
							character->moveDirection(WEST);
					}

					else if (cDirection == NORTH || cDirection == SOUTH){
						if( (center->x < enemyCenter->x) && 
						(enemyCenter->y+distanceY > center->y) &&
						(enemyCenter->y-distanceY < center->y) )
							character->moveDirection(WEST);
						else{
							if(cDirection == NORTH)
								character->moveDirection(SOUTH);
							else 
								character->moveDirection(NORTH);
						}
					}
					break;
				case SOUTH:
					if(cDirection == SOUTH || cDirection == NORTH){
						if(center->y > enemyCenter->y)
							character->moveDirection(SOUTH);
						else
							character->moveDirection(NORTH);
					}

					else if(cDirection == EAST || cDirection == WEST){
						if( (center->y > enemyCenter->y) &&
							(enemyCenter->x-distanceX < center->x) &&
							(enemyCenter->x+distanceX > center->x))
							character->moveDirection(SOUTH);
						else{
							if(cDirection == EAST)
								character->moveDirection(WEST);
							else
								character->moveDirection(EAST);
						}
					}
					break;
				case NORTH:
					if(cDirection == SOUTH || cDirection == NORTH){
						if(center->y < enemyCenter->y)
							character->moveDirection(NORTH);
						else
							character->moveDirection(SOUTH);
					}

					else if(cDirection == EAST || cDirection == WEST){
						if( (center->y < enemyCenter->y) &&
							(enemyCenter->x-distanceX < center->x) &&
							(enemyCenter->x+distanceX > center->x))
							character->moveDirection(NORTH);
						else{
							if(cDirection == EAST)
								character->moveDirection(WEST);
							else
								character->moveDirection(EAST);
						}
					}
					break;
				}
			}
		}
	}

}

////////////////////////////////////////////////////////

bool FC World::composeFrame(){
	//if(checkLevelComplete()) changeGameState(GameComplete);
	advanceEnemies();
	advanceBullets();
	checkCharacterCollision();
	return true;	
}

bool World::checkLevelComplete(){
	return scene1->isCompleted() && scene2->isCompleted() && scene3->isCompleted()
		&& scene4->isCompleted() && scene5->isCompleted() && scene6->isCompleted()
		&& scene7->isCompleted();
}
////////////////////////////////////////////////////////

bool FC World::draw(LPDDSurface surface){	

	BoundingBox *charFeetBox = character->getFeetBoundingBox();	
	BoundingBox *box;
	
	if(charFeetBox->bottom >= SCREEN_HEIGHT)
		box = new BoundingBox(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
	else box = new BoundingBox(0, 0, charFeetBox->bottom+1, SCREEN_WIDTH);

	//if(!currScene->draw(surface, box, box)) return false;
	currScene->draw(surface, box, box);


	if(character->getCurrentDirection() == SOUTH){
		character->animate();
		if(!character->draw(surface)) return false;	

		box = new BoundingBox(charFeetBox->bottom, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
		//if(!currScene->draw(surface, box, box))return false;
		currScene->draw(surface, box, box);

		for(BulletList::iterator i = bulletList.begin(); i != bulletList.end(); i++)
			if(!(*i)->draw(surface)) {
				dprintf(("  World::draw(): could not draw bullet\n"));
			return false;
		}
	}
	else{
		// animate and draw the character
		character->animate();
		if(!character->draw(surface)) return false;	

		for(BulletList::iterator i = bulletList.begin(); i != bulletList.end(); i++)
			if(!(*i)->draw(surface)) {
				dprintf(("  World::draw(): could not draw bullet\n"));
			return false;
		}
	 
		box = new BoundingBox(charFeetBox->bottom, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
		//if(!currScene->draw(surface, box, box))return false;
		currScene->draw(surface, box, box);

	
	}


	for(BulletList::iterator j = enemyBulletList.begin(); j != enemyBulletList.end(); j++)
		if(!(*j)->draw(surface)) {
			dprintf(("  World::draw(): could not draw bullet\n"));
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////

bool World::createBullet(){
	dprintf(("entering World::createBullet()\n"));
	if(GetTickCount() - lastBulletTime > bulletCounter){
		lastBulletTime=GetTickCount();
		Bullet *bullet = new Bullet(BULLET_FILE,BULLET_FRAMES,BULLET_WIDTH,
							    BULLET_HEIGHT,BULLET_ROWS,BULLET_COLUMNS, CharBullet);
		bullet->setSpeed(15,15);
		bullet->setDirection(character->getCurrentDirection());
		bullet->setPosition(character->getArmPosition());
		bulletList.insert(bulletList.begin(), bullet);
		//bulletList.push_front(bullet);
		return true;
	}
	return false;
	dprintf(("leaving createBullet()"));
}

////////////////////////////////////////////////////////

bool World::createEnemyBullet(Enemy* enemy){
	dprintf(("entering World::createEnemyBullet()\n"));
	if(GetTickCount() - lastBulletTime > bulletCounter){
		lastBulletTime=GetTickCount();
		Bullet *bullet = new Bullet(EBULLET_FILE,EBULLET_FRAMES,EBULLET_WIDTH,
							    EBULLET_HEIGHT,EBULLET_ROWS,EBULLET_COLUMNS, EnemyBullet);
		bullet->setSpeed(15,15);
		bullet->setDirection(enemy->getCurrentDirection());
		bullet->setPosition(enemy->getArmPosition());
		enemyBulletList.insert(enemyBulletList.begin(), bullet);
		return true;
	}
	return false;
	dprintf(("leaving createEnemyBullet()"));
}

////////////////////////////////////////////////////////

bool World::restore(){	
	//currScene->Scene::restore();
	scene1->restore();
	scene2->restore();
	scene3->restore();
	scene4->restore();
	scene5->restore();
	scene6->restore();
	scene7->restore();
	if(character->restore()){
		character->load(CHAR_FILE_NAME);
		character->loadSprites();
	}
	return true;
}

void World::release(){
	if(scene1) scene1->release();
	if(scene2) scene2->release();
	if(scene3) scene3->release();
	if(scene4) scene4->release();
	if(scene5) scene5->release();
	if(scene6) scene6->release();
	if(scene7) scene7->release();
	if(currScene) currScene->release(); // releasing twice just in case
}