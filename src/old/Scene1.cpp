// Scene1.cpp: implementation of the Scene1 class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene1.h"
#include "TrashCan.h"
#include "debug.h"
#include "World.h"
#include "HDoor.h"
#include "VDoor.h"
#include "files.h"
#include "Character.h"
#include "InvisibleEntity.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Entity *trashCan = new TrashCan();
Entity *trashCan = new Entity(32, 64);
Door *door1 = new HDoor(H_DOOR_WIDTH, H_DOOR_HEIGHT);
Door *door2 = new VDoor(V_DOOR_WIDTH, V_DOOR_HEIGHT);
Entity *health1 = new Entity(HEALTH_WIDTH, HEALTH_HEIGHT);

// action->data must be a Character object
void giveHealth(Action *action){
	dprintf(("*************** giveHealth() ***************\n"));		
	action->source->playSound(); // the health objects sound
	Character *c = (Character *)(action->data);
	c->changeHealth(20);	
}

// action->data must be a Scene object
void removeEntity(Action *action){
	dprintf(("*************** removeEntity() ***************\n"));			
	Scene *scene = (Scene *)(action->data);
	scene->entities.remove(action->source);
	
}


void activateSwitch(Action *action){
	dprintf(("*************** switch activated ***************\n"));		
	//Scene *scene = (Scene *)(action->data);
	Door *door = (Door *)(action->data);
	//action->source->animate();
	//scene->entities.remove(trashCan);	
	action->source->playSound();
	door->toggle();
}


// ayction->data must be a Door
void toggleDoor(Action *action){
	dprintf(("*************** toggle Door ***************\n"));
	Door *door = (Door *)(action->data);
	door->toggle();
	//door1->toggle();
	
	//source->setSceneCoord(source->getX()-source->getWidth(), source->getY());
	//action->source->toggle();
	// set the tile map to passable as the door opens
}

/////////////////////////////////////////////////////

Scene1::Scene1(char *dataFile) : Scene(dataFile){
		
}

Scene1::~Scene1(){

}

/////////////////////////////////////////////////////

bool Scene1::init(){
	
	completed = true;

	// init and insert enemies here
	

	//insert health
	health1->bmpFileName = HEALTH_FILE_NAME;
	health1->setSceneCoord(74, 64);
	health1->setAttribute(ENTITY_PICKUPABLE, true);
	health1->soundFileName = "sounds/bonus.wav";
	entities.insert(entities.begin(), health1);

	//insert trashcan	
	trashCan->bmpFileName = "images/trashcan.bmp";
	trashCan->setSceneCoord(320, 200);	
	trashCan->setAttribute(ENTITY_MOVEABLE, true);
	entities.insert(entities.begin(), trashCan);

	Entity *keypad= new InvisibleEntity(32, 32);
	keypad->setSceneCoord(352, 0);	
	keypad->soundFileName = "sounds/doorbutton.wav";
	entities.insert(entities.begin(), keypad);	
	
	//insert door1
	door1->bmpFileName = H_DOOR_FILE_NAME;		
	door1->setSceneCoord(256, 12);	
	entities.insert(entities.begin(), door1);	
	//doors.insert(doors.begin(), door1);
	
	//insert door2
	door2->bmpFileName = V_DOOR_FILE_NAME;
	door2->setSceneCoord(SCREEN_WIDTH-door2->getWidth()-10, 176);
	entities.insert(entities.begin(), door2);
	//door2->toggle();
	//doors.insert(doors.begin(), door2);
		
	//keypad->actionList.insert(keypad->actionList.end(), 
		//new Action(ACTION_PUSH, keypad, toggleDoor, door1));
	keypad->actionList.insert(keypad->actionList.end(), 
		new Action(ACTION_PUSH, keypad, activateSwitch, door1));

	keypad->actionList.insert(keypad->actionList.end(), 
		new Action(ACTION_PUSH, keypad, toggleDoor, door2));

	health1->actionList.insert(health1->actionList.end(),
		new Action(ACTION_PICKUP, health1, giveHealth, character));
	
	health1->actionList.insert(health1->actionList.end(),
		new Action(ACTION_PICKUP, health1, removeEntity, this));
	
	// init and insert enemies here	
	Enemy *enemy = new Enemy(ENEMY1);	
	enemy->setSpeed(3);
	enemy->setPosition(400,250);
	enemy->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy);

	// call the super class' init function which does all other initialization
	if(!this->Scene::init()) return false; 
	initialized = true;
	return true;
}



bool Scene1::load(){
	dprintf(("entered Scene1::load()\n"));
	if(!initialized) return false;
	dprintf(("  about to load parent class()\n"));
	if(!this->Scene::load()) return false; // call the super class' load function

	//insert other entities
	dprintf(("leaving Scene1::load()\n"));
	return true;
}

bool FC Scene1::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){
	if(!Scene::draw(surface, destBox, srcBox))
		return false;
	return true;
}

void Scene1::addEnemy(){
	if(enemies.size() == 0){
		Enemy *enemy = new Enemy(ENEMY1);
		enemy->init();
		enemy->setSpeed(3);
		enemy->setPosition(400,250);
		enemy->setAnimationSpeed(200);
		enemies.insert(enemies.begin(), enemy);
	}
}

