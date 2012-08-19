// Scene3.cpp: implementation of the Scene3 class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene3.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scene3::Scene3(char *dataFile) : Scene(dataFile){

}

Scene3::~Scene3()
{

}

bool Scene3::init(){

	completed = true;
	Enemy *enemy = new Enemy(ENEMY1);	
	enemy->setSpeed(3);
	enemy->setPosition(400,250);
	enemy->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy);

	if(!this->Scene::init()) return false; 
	initialized = true;
	return true;
}

bool FC Scene3::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){
	if(!Scene::draw(surface, destBox, srcBox))
		return false;
	return true;
}

void Scene3::addEnemy(){
	if(enemies.size() == 0){
		Enemy *enemy = new Enemy(ENEMY1);		
		enemy->init();
		enemy->setSpeed(3);
		enemy->setPosition(400,250);
		enemy->setAnimationSpeed(200);
		enemies.insert(enemies.begin(), enemy);
	}
}
