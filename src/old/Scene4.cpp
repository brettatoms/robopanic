// Scene4.cpp: implementation of the Scene4 class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene4.h"
#include "VDoor.h"
#include "files.h"
#include "Character.h"
#include "BoundingBox.h"
#include "InvisibleEntity.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Door *door3 = new VDoor(V_DOOR_WIDTH, V_DOOR_HEIGHT);
Entity *health2 = new Entity(HEALTH_WIDTH, HEALTH_HEIGHT);
Entity *box1 = new Entity(32,32);
Entity *box2 = new Entity(31,31);
Entity *box3 = new Entity(31,31);
Entity *box4 = new Entity(31,31);
InvisibleEntity *stepBox = new InvisibleEntity(32,640);

extern void toggleDoor(Action *action);
extern void giveHealth(Action *action);
extern void removeEntity(Action *action);
extern void activateSwitch(Action *action);		

Scene4::Scene4(char *dataFile) : Scene(dataFile){
	puzzle = false;
}

Scene4::~Scene4(){

}


bool Scene4::init(){
	completed = true;
	//insert a door
	door3->bmpFileName = V_DOOR_FILE_NAME;		
	door3->setSceneCoord(11,176);	
	door3->toggle();	
	entities.insert(entities.begin(), door3);	

	//stepBox->fileName = ("images//stepbox.bmp");
	stepBox->setSceneCoord(128, 0);
	stepBox->setAttribute(ENTITY_PICKUPABLE, true);
	entities.insert(entities.begin(), stepBox);

	health2->bmpFileName = HEALTH_FILE_NAME;
	health2->setSceneCoord(52, 64);
	health2->setAttribute(ENTITY_PICKUPABLE, true);
	health2->soundFileName = "sounds/bonus.wav";
	entities.insert(entities.begin(), health2);

	health2->actionList.insert(health2->actionList.end(),
		new Action(ACTION_PICKUP, health2, giveHealth, character));
	
	health2->actionList.insert(health2->actionList.end(),
		new Action(ACTION_PICKUP, health2, removeEntity, this));

	stepBox->actionList.insert(stepBox->actionList.end(),
		new Action(ACTION_PICKUP, stepBox, removeEntity, this));

	stepBox->actionList.insert(stepBox->actionList.end(),
		new Action(ACTION_PICKUP, stepBox, toggleDoor, door3));


	box1->bmpFileName = "images/box1.bmp";
	//box1->setSceneCoord(120,96);	
	box1->setSceneCoord(96, 200);
	box1->setAttribute(ENTITY_MOVEABLE, true);
	entities.insert(entities.begin(), box1);

	/*box2->fileName = "images/box2.bmp";
	box2->setSceneCoord(480,96);	
	box2->setAttribute(ENTITY_MOVEABLE, true);
	entities.insert(entities.begin(), box2);

	box3->fileName = "images/box3.bmp";
	box3->setSceneCoord(128, 352);	
	box3->setAttribute(ENTITY_MOVEABLE, true);
	entities.insert(entities.begin(), box3);

	box4->fileName = "images/box4.bmp";
	box4->setSceneCoord(480, 352);	
	box4->setAttribute(ENTITY_MOVEABLE, true);
	entities.insert(entities.begin(), box4);*/

	//square1 = new BoundingBox(240,288,240+35,288+35);
	square1 = new BoundingBox(352,94,352+38,94+38);

	//create the enemy
	/*
	Enemy *enemy = new Enemy(ENEMY1);	
	enemy->setSpeed(3);
	enemy->setPosition(70,200);
	enemy->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy);
	*/

	// background sound file name
	backgroundSoundFileName = "sounds\\machineroom1.wav";

	if(!this->Scene::init()) return false; 
	initialized = true;
	return true;
}


bool FC Scene4::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){
	if(puzzle==false)
		checkPuzzle();
	if(!Scene::draw(surface, destBox, srcBox))
		return false;
	return true;
}


bool Scene4::checkPuzzle(){
	BoundingBox* temp = box1->getBaseBoundingBox();
	if(temp->top > square1->top && temp->bottom < square1->bottom
		&& temp->left > square1->left && temp->right < square1->right)
		puzzle=true;
	if(puzzle==true){
		door3->toggle();
		dprintf(("SMITTY"));
		box1->setAttribute(ENTITY_MOVEABLE, false);
		//box1->setSceneCoord(288,224);
		box1->setSceneCoord(square1->left,square1->top);
		completed = true;
	}
	return true;
}

void Scene4::addEnemy(){
	if(enemies.size() == 0 && hasEnemies == true){
		Enemy *enemy = new Enemy(ENEMY1);
		enemy->init();
		enemy->setSpeed(3);
		enemy->setPosition(70,200);
		enemy->setAnimationSpeed(200);
		enemies.insert(enemies.begin(), enemy);
	}
}