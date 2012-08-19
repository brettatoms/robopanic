// Scene7.cpp: implementation of the Scene7 class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene7.h"
#include "debug.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




Scene7::Scene7(char *dataFile) : Scene(dataFile){

}

Scene7::~Scene7(){

}


bool Scene7::init(){
	Enemy *enemy = new Enemy(ENEMY1);	
	enemy->setSpeed(3);
	enemy->setPosition(400,250);
	enemy->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy);

	//dprintf(("inited enemy1\n"));

	Enemy *enemy2 = new Enemy(ENEMY1);	
	enemy2->setSpeed(4);
	enemy2->setPosition(100,100);
	enemy2->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy2);
	//dprintf(("inited enemy2\n"));

	Enemy *enemy3 = new Enemy(ENEMY1);	
	enemy3->setSpeed(3);
	enemy3->setPosition(400,100);
	enemy3->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy3);
	//dprintf(("inited enemy3\n"));

	Enemy *enemy4 = new Enemy(ENEMY1);	
	enemy4->setSpeed(5);
	enemy4->setPosition(50,350);
	enemy4->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy4);
	//dprintf(("inited enemy4\n"));

	Enemy *enemy5 = new Enemy(ENEMY1);	
	enemy5->setSpeed(4);
	enemy5->setPosition(100,300);
	enemy5->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy5);
	//dprintf(("inited enemy5\n"));

    Enemy *enemy6 = new Enemy(ENEMY1);
	enemy6->setSpeed(6);
	enemy6->setPosition(300,50);
	enemy6->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy6);
	//dprintf(("inited enemy6\n"));

    Enemy *enemy7 = new Enemy(ENEMY1);	
	enemy7->setSpeed(5);
	enemy7->setPosition(200,100);
	enemy7->setAnimationSpeed(200);
	enemies.insert(enemies.begin(), enemy7);


	//dprintf(("inited enemy7\n"));
	if(!this->Scene::init()) return false; 
	initialized = true;
	return true;
}

bool FC Scene7::draw(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox){
	if(enemies.size() == 0) completed = true;
	if(!Scene::draw(surface, destBox, srcBox))
		return false;
	return true;
}

void Scene7::addEnemy(){
	if(enemies.size() == 0){
		Enemy *enemy = new Enemy(ENEMY1);
		enemy->init();
		enemy->setSpeed(3);
		enemy->setPosition(400,250);
		enemy->setAnimationSpeed(200);
		enemies.insert(enemies.begin(), enemy);
	}
}