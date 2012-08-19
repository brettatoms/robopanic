// Scene6.cpp: implementation of the Scene6 class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene6.h"
#include "defines.h"
#include "files.h"

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include "Scene7.h"
#include "VDoor.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern Scene1 *scene1;
extern Scene2 *scene2;
extern Scene3 *scene3;
extern Scene4 *scene4;
extern Scene5 *scene5;
extern Scene6 *scene6;

VDoor *door1 = new VDoor(V_DOOR_WIDTH, V_DOOR_HEIGHT);

/*
void toggleDoor(Action *action){
	

}*/

Scene6::Scene6(char *dataFile) : Scene(dataFile){


}

Scene6::~Scene6(){

}


bool Scene6::init(){
	//completed = true;
	Enemy *enemy = new Enemy(ENEMY1);	
	enemy->setSpeed(3);
	enemy->setPosition(200,250);
	enemy->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy);

	door1->bmpFileName = V_DOOR_FILE_NAME;	
	door1->setSceneCoord(11, 80);
	entities.insert(entities.begin(), door1);

	if(!this->Scene::init()) return false; 
	initialized = true;
	return true;
}


bool FC Scene6::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){
	if(scene1->isCompleted() && scene2->isCompleted() && scene3->isCompleted()
		&& scene4->isCompleted() && scene5->isCompleted() && !completed) {
		completed = true;
		door1->toggle();
	}

	if(!Scene::draw(surface, destBox, srcBox))
		return false;
	return true;
}

void Scene6::addEnemy(){
	if(enemies.size() == 0){
		Enemy *enemy = new Enemy(ENEMY1);
		enemy->init();
		enemy->setSpeed(3);
		enemy->setPosition(200,250);
		enemy->setAnimationSpeed(200);
		enemies.insert(enemies.begin(), enemy);
	}
}