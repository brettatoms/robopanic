// Scene2.cpp: implementation of the Scene2 class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene2.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scene2::Scene2(char *sceneDataFileName) : Scene(sceneDataFileName){


}

Scene2::~Scene2()
{

}


bool Scene2::init(){
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

bool FC Scene2::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){
	if(!Scene::draw(surface, destBox, srcBox))
		return false;
	return true;
}

void Scene2::addEnemy(){
	if(enemies.size() == 0){
		Enemy *enemy = new Enemy(ENEMY1);
		enemy->init();
		enemy->setSpeed(3);
		enemy->setPosition(400,250);
		enemy->setAnimationSpeed(200);
		enemies.insert(enemies.begin(), enemy);
	}
}
