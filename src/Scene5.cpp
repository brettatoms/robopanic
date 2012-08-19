// Scene5.cpp: implementation of the Scene5 class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene5.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scene5::Scene5(char *dataFile) : Scene(dataFile)
{

}

Scene5::~Scene5()
{

}


bool Scene5::init(){
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

bool FC Scene5::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){
	if(!Scene::draw(surface, destBox, srcBox))
		return false;
	return true;
}

void Scene5::addEnemy(){
	if(enemies.size() == 0){
		Enemy *enemy = new Enemy(ENEMY1);
		enemy->init();
		enemy->setSpeed(3);
		enemy->setPosition(400,250);
		enemy->setAnimationSpeed(200);
		enemies.insert(enemies.begin(), enemy);
	}
}