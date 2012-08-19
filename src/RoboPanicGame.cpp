// RoboPanicGame.cpp: implementation of the RoboPanicGame class.
//
//////////////////////////////////////////////////////////////////////

#include "RoboPanicGame.h"
#include "World.h"
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include "defines.h" 
#include "scene.h"
#include "character.h"
#include "debug.h"
#include "SceneCoordinate.h"
#include "BmpLoader.h"
#include "Interface.h"
#include "ActionMenu.h"
#include "Bullet.h"
#include "Sound.h"
#include "Timer.h"

UCHAR keyboard_state[256];

CSoundManager *soundManager;
CTimer timer;
int stateTime = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RoboPanicGame::RoboPanicGame(int screenWidth, int screenHeight, int colorDepth)
	: DXGame(screenWidth, screenHeight, colorDepth){
	world = NULL;
	//changeGameState(GameIntro);
	gameInterface= NULL;
	lastComposeTime=0;
	framesPerSecond=30;
	waitTime=1000/framesPerSecond;
	titleScreen = NULL;
	creditScreen = NULL;
	howPlayScreen = NULL;
	levelScreen = NULL;
	completeScreen = NULL;
	preGameScreen = NULL;	
	pauseScreen = NULL;
	startText = NULL;
	continueText = NULL;
}

RoboPanicGame::~RoboPanicGame(){
	release();
	titleScreen = NULL;	
	creditScreen = NULL;
	howPlayScreen = NULL;
	levelScreen = NULL;
	completeScreen = NULL;
	preGameScreen = NULL;	
	pauseScreen = NULL;
	startText = NULL;
	continueText = NULL;
	if(soundManager) delete soundManager;
	if(world) delete world;		
}

//////////////////////////////////////////////////////////

// do anything that needs to be done when going from one game state
// to the next, e.g. load and unload sounds, etc
/*
void RoboPanicGame::changeGameState(GameState gameState){
	timer.start();
	prevGameState=this->gameState;	
	switch(gameState){
	case PreGame:
	    if(prevGameState == GameTitle)
			soundManager->stop();
		soundManager->play(PRE_SOUND);
		break;
	case GameTitle:
		if(prevGameState == PreGame) {
			soundManager->stop();
	        soundManager->play(PANIC_SOUND);
		}	
		if(prevGameState != GameIntro)
		    soundManager->play(INTRO_SOUND,true);
		break;
	case GameIntro:
		if(prevGameState == GamePlaying || prevGameState == GameComplete)
			soundManager->stop();
		ShowCursor(TRUE);
		if(prevGameState != HowScreen && prevGameState != CreditScreen) {   		
			soundManager->play(INTRO_SOUND, true);
		}
		break;
		case LevelScreen:
		break;
	case HowScreen:
		break;
	case CreditScreen:
		break;		
	case GameChangingScene:		
		break;
	case GamePlaying:	
		ShowCursor(FALSE);
		if(prevGameState == LevelScreen || prevGameState == GamePaused || 
			prevGameState == GameComplete)		
			soundManager->stop();
		if(world->prevScene->backgroundSoundFileName != NULL) // stop the sounds
			soundManager->stop(world->prevScene->sound);	// from prev scene
		if(world->currScene->backgroundSoundFileName != NULL){ // there is a sound
			soundManager->stop();
			world->currScene->playBackgroundSound();
		}else soundManager->play(SCENE_BG_SOUND, true);
		break;
	case GamePaused:
		if(prevGameState == GamePlaying)
			soundManager->stop();
		soundManager->play(PAUSED_SOUND, true);
		break;
	case GameComplete:
		soundManager->stop();
		soundManager->play(COMPLETE_SOUND, true);
		break;
	default:
		dprintf((" unknown games state: %d\n", gameState));
		return;		
	}
	this->gameState = gameState;
	stateTime = timer.time();
}
*/
void RoboPanicGame::changeGameState(GameState gameState){
	prevGameState=this->gameState;	
	switch(gameState){
	case StartMenu:
		if(prevGameState != HowScreen && prevGameState != CreditScreen) {   		
			soundManager->play(INTRO_SOUND, true);
			soundManager->play(PANIC_SOUND);
		}
		//soundManager->play(PANIC_SOUND);
		break;
	case ContinueMenu:
		if(prevGameState == GamePlaying || prevGameState == GameComplete || prevGameState == GamePaused)
			soundManager->stop();
		if(prevGameState != HowScreen && prevGameState != CreditScreen)
			soundManager->play(PANIC_SOUND);
		soundManager->play(INTRO_SOUND, true);
		ShowCursor(TRUE);
		break;
	case LevelScreen:
		break;
	case HowScreen:
		break;
	case CreditScreen:
		break;		
	case GameChangingScene:		
		break;
	case GamePlaying:	
		ShowCursor(FALSE);
		if(prevGameState == LevelScreen || prevGameState == GamePaused || 
			prevGameState == GameComplete || prevGameState == ContinueMenu)		
			soundManager->stop();
		
		if(world->prevScene->backgroundSoundFileName != NULL) // stop the sounds
			soundManager->stop(world->prevScene->sound);	// from prev scene
		if(world->currScene->backgroundSoundFileName != NULL){ // there is a sound
			soundManager->stop();
			world->currScene->playBackgroundSound();
		}else{			
			soundManager->play(SCENE_BG_SOUND, true);
		}		
		break;		
	case GamePaused:
		if(prevGameState == GamePlaying)
			soundManager->stop();
		soundManager->play(PAUSED_SOUND, true);
		break;
	case GameComplete:
		soundManager->stop();
		soundManager->play(COMPLETE_SOUND, true);
		break;
	default:
		dprintf((" unknown games state: %d\n", gameState));
		return;		
	}
	this->gameState = gameState;
	stateTime = timer.time();
}



//////////////////////////////////////////////////////////

bool RoboPanicGame::initialize(HINSTANCE hInstance, void* winProc, char *className){
	dprintf(("RoboPanicGame::initialize()\n"));

	// super class initializes windows and directX
	if(!this->DXGame::initialize(hInstance, winProc, className)){	
		return FALSE;
	}

	soundManager = new CSoundManager(hwnd);
	
	titleScreen = new BmpLoader();
	titleScreen->load("images/title.bmp");	

	startText = new BmpLoader();
	startText->load("images/startmtext.bmp");

	continueText = new BmpLoader();
	continueText->load("images/contmtext.bmp");

	creditScreen =  new BmpLoader();
	creditScreen->load("images/credits.bmp");	

	pauseScreen = new BmpLoader();
	pauseScreen->load("images/pausemenu.bmp");
	
	howPlayScreen =  new BmpLoader();
	howPlayScreen->load("images/howplay.bmp");
	
	levelScreen =  new BmpLoader();
	levelScreen->load("images/level.bmp");
	
	completeScreen =  new BmpLoader();
	completeScreen->load("images/complete.bmp");

	if(gameInterface== NULL) gameInterface= new Interface();
	gameInterface->init();

	world = new World();
	if(!world->initialize()){
		DestroyWindow(hwnd);
		return FALSE;
	}
		
	initSounds();
	changeGameState(StartMenu);

	dprintf(("leaving RoboPanicGame::initialize()\n"));
	return TRUE;	
}

//////////////////////////////////////////////////////////

void clearSurface(LPDDSurface); // defined in ddutil.cpp



void RoboPanicGame::composeFrame(){
	//const int INTRO_TIME = 14000; 
    //const int TITLE_TIME = 10000; 
	const int LEVEL_TIME = 2000;
    const int COMPLETE_TIME = 10000; 

	//dprintf(("entered RoboPanicGame::composeFrame()\n"));	
	switch(gameState){
	case StartMenu: 
		ShowCursor(TRUE);
		titleScreen->draw(lpSecondary, 0, 0);
		startText->draw(lpSecondary, 35, 250);
		//world->character->changeHealth(100);
		//world->character->setCharacterState(ALIVE);
		pageFlip();
		break;
	case ContinueMenu:
		ShowCursor(TRUE);
		titleScreen->draw(lpSecondary, 0, 0);
		continueText->draw(lpSecondary, 35, 250);
		world->character->changeHealth(100);
		world->character->setCharacterState(ALIVE);
		pageFlip();
		break;
	case LevelScreen:
		
        levelScreen->draw(lpSecondary, 0, 0);
		//world->character->changeHealth(100);
		//world->character->setCharacterState(ALIVE);
		if(timer.elapsed(stateTime, LEVEL_TIME))
		   changeGameState(GamePlaying);
		pageFlip();
		break;
		
	case HowScreen:
		howPlayScreen->draw(lpSecondary, 0, 0);
		pageFlip();
		break;	
	case CreditScreen:
		creditScreen->draw(lpSecondary,0,0);
		pageFlip();
		break;
	case GamePaused:
		pauseScreen->draw(lpPrimary,170,180, DDBLT_WAIT|DDBLT_KEYSRC);
		//drawText("GamePaused",200, 100, RGB(0,0,0), lpPrimary);		
		break;
	case GamePlaying:
		ShowCursor(FALSE);		
		if(world->checkLevelComplete()) changeGameState(GameComplete);
		if(GetTickCount() - lastComposeTime > waitTime){
			if(world->character->getCharacterState() == DEAD)
				changeGameState(ContinueMenu);
            else if(world->character->getCharacterState() == HIT)
				soundManager->play(HIT_CHAR_SOUND);
			lastComposeTime=GetTickCount();
			world->character->setCharacterMoving(false);
			clearSurface(lpSecondary);
			lpKeyboard->GetDeviceState(256, (LPVOID)keyboard_state);
			directInputHandler();
			
			if(world->composeFrame()){
				if(!world->draw(lpSecondary))
					dprintf(("  ! world->draw\n"));
			}else { dprintf(("  ! world->composeFrame\n")); }
			
			if(!gameInterface->draw(lpSecondary, world)){
				dprintf(("  ! draw->interface\n"));		
			}
			if(world->hasSceneChanged()) changeGameState(GameChangingScene);
			else pageFlip();
		}
		break;
	case GameChangingScene:
		if(GetTickCount() - lastComposeTime > waitTime){
			clearSurface(lpSecondary);
			scrollInNewScene(lpSecondary);
			pageFlip();
		}
		break;
	case GameComplete:
		completeScreen->draw(lpSecondary, 0, 0);
		if(timer.elapsed(stateTime, COMPLETE_TIME)) 
           changeGameState(ContinueMenu); 
		else pageFlip();
		break;
	}

	//dprintf(("leaving RoboPanicGame::composeFrame()\n"));	
}
//////////////////////////////////////////////////////////

void FC RoboPanicGame::scrollInNewScene(LPDDSurface surface){
	//dprintf(("**************** entered World::scrollInNewScene()\n"));	
	if(world->scrollInNewScene(surface))    // returns true when done scrolling
		changeGameState(GamePlaying);
}


//////////////////////////////////////////////////////////

void RoboPanicGame::initSounds(){ //init sounds for game
    //dprintf(("entered RoboPanicGame::initSounds()\n"));
	const int copies=4; //copies of repeatable sounds
	soundManager->load("sounds/sefx1.wav", PRE_SOUND);
	soundManager->load("sounds/ambient_menu.wav", INTRO_SOUND);	
	soundManager->load("sounds/loop3.wav", SCENE_BG_SOUND); //was loop7.wav or loop3		
	soundManager->load("sounds/kickitdown.wav", PAUSED_SOUND);
	soundManager->load("sounds/panic.wav", PANIC_SOUND);
	soundManager->load("sounds/gunshot.wav", GUN_SOUND);
	soundManager->load("sounds/gunshot.wav", CHANGE_SCENE_SOUND);
	soundManager->load("sounds/loop8.wav", COMPLETE_SOUND);
	soundManager->load("sounds/pain.wav", HIT_CHAR_SOUND);
	soundManager->load("sounds/nautalis.wav", HIT_PLAYER_SOUND);
	//dprintf(("entered RoboPanicGame::initSounds()\n"));	
}  // end of loadSounds

//////////////////////////////////////////////////////////

void RoboPanicGame::keyboardHandler(UINT message, WPARAM key){
	dprintf(("entered RoboPanicGame::keyboardHandler()\n"));	
	SceneCoordinate center;	
	if(message == WM_KEYDOWN){ // handle keydown messages
		switch(key){		
		case VK_SPACE:
			//if(gameState == PreGame) changeGameState(GameTitle);
			//if(gameState == GameTitle) changeGameState(GameIntro);
			if(gameState == GameComplete) changeGameState(StartMenu);
			else if(gameState == LevelScreen) changeGameState(GamePlaying);
			break;
		case 'z':
			soundManager->stop();
			break;
		case 'C':
			if(gameState == ContinueMenu) 
				changeGameState(GamePlaying);			
			break;	
		case 'S':
			if(gameState == StartMenu) 
				changeGameState(LevelScreen);
			break;	
		case VK_ESCAPE:
			if(gameState == HowScreen || gameState == CreditScreen){
				if(prevGameState == StartMenu)
					changeGameState(StartMenu);
				else
					changeGameState(ContinueMenu);
			}			
			break;
		case 'O':
			if(gameState == StartMenu || gameState == ContinueMenu) 
				changeGameState(HowScreen);	
			break;
		case 'Q':
			if(gameState == StartMenu || gameState == ContinueMenu)
				DestroyWindow(hwnd);
			if(gameState == GamePaused)
				changeGameState(ContinueMenu);
			break;
		case 'H':
	//		//if(gameState == StartMenu || gameState == ContinueMenu) changeGameState(HowScreen);	
			if(gameState == GamePlaying) world->character->changeHealth(5);
			break;
		case 'A':
			world->reactToAction(ACTION_PUSH);
			break;
		case 'P':
			//if(gameState == GamePaused) changeGameState(GamePlaying);	
			//else 
			if(gameState == GamePlaying) changeGameState(GamePaused);			
			break;
		//case 'M':
		//	if(gameState == GamePaused)
		//		changeGameState(ContinueMenu);
		//	break;
		case 'R':
			if(gameState == GamePaused)
				changeGameState(GamePlaying);
			if(gameState == StartMenu || gameState == ContinueMenu) 
				changeGameState(CreditScreen);
			break;
		case 'K':
			world->character->changeHealth(-20);
			break;
		case 'X':
			if(gameState == GamePlaying) changeGameState(GameComplete);
			break;
		}
	}
}

//////////////////////////////////////////////////////////

void RoboPanicGame::directInputHandler(){
	if(gameState == GamePlaying){
		if (keyboard_state[DIK_F]){
			if(world->createBullet()){
				dprintf(("*********** HIT THE F KEY ***********"));
				world->character->setCharacterState(SHOOTING);			
				soundManager->play(GUN_SOUND); // the number of the laser
			}
		}
		else if (keyboard_state[DIK_UP]){		
			world->character->setCharacterMoving(true);
			
			if(world->getCharacterDirection() != NORTH)
				world->setCharacterDirection(NORTH);
			if(world->character->getCharacterState() != DYING && 
				world->character->getCharacterState() != DEAD)
				world->advanceCharacter();
		}

		else if (keyboard_state[DIK_RIGHT]){	
			world->character->setCharacterMoving(true);
			if(world->getCharacterDirection() != EAST)
				world->setCharacterDirection(EAST);
			if(world->character->getCharacterState() != DYING && 
				world->character->getCharacterState() != DEAD)
				world->advanceCharacter();
		}

		else if (keyboard_state[DIK_DOWN]){	
			world->character->setCharacterMoving(true);			
			if(world->getCharacterDirection() != SOUTH)
				world->setCharacterDirection(SOUTH);
			if(world->character->getCharacterState() != DYING && 
				world->character->getCharacterState() != DEAD)
				world->advanceCharacter();
		}

		else if (keyboard_state[DIK_LEFT]){
			world->character->setCharacterMoving(true);			
			if(world->getCharacterDirection() != WEST)
				world->setCharacterDirection(WEST);
			if(world->character->getCharacterState() != DYING && 
				world->character->getCharacterState() != DEAD)
				world->advanceCharacter();
		}	
	}// end GamePlaying state
}

//////////////////////////////////////////////////////////

void RoboPanicGame::mouseButtonHandler(UINT message, WPARAM wParam, LPARAM lParam){
	int x, y;
	static int old_x, old_y;
	x = LOWORD(lParam);	y = HIWORD(lParam);	
	EntityList *entities = new EntityList();
	int num_entities = 0;
	dprintf(("   ----> mouse button: %d, %d\n", x, y));
	switch(message){
	case WM_LBUTTONDOWN:
		if(gameState == StartMenu){
			if(x>36 && x<145 && y>255 && y<276)
				changeGameState(LevelScreen);
			else if(x>37 && x<196 && y>283 && y<305)
				changeGameState(HowScreen);
			else if(x>37 && x<184 && y>312 && y<332)
				changeGameState(CreditScreen);
			else if(x>37 && x<113 && y>339 && y<360)
				DestroyWindow(hwnd);
		}

		else if(gameState == ContinueMenu){
			if(x>38 && x<208 && y>255 && y<275)
				changeGameState(GamePlaying);
			else if(x>37 && x<196 && y>283 && y<305)
				changeGameState(HowScreen);
			else if(x>37 && x<184 && y>312 && y<332)
				changeGameState(CreditScreen);
			else if(x>37 && x<113 && y>339 && y<360)
				DestroyWindow(hwnd);
			dprintf(("COORDS%d%d\n",x,y));
		}

		else if(gameState == GamePlaying){
			num_entities = world->getEntitiesAt(x, y, entities);
			dprintf(("  found %d entites\n", num_entities));
			if(num_entities > 0){
				if(!gameInterface->showActionMenu((*entities->begin()), x, y)){
					dprintf(("  ** Error: problems creating entity action menu\n"));
				}
			}	
		} // playing state
		break;
		
	case WM_LBUTTONUP:
		if(gameState == GamePlaying){
			//if(gameInterface->isActionMenuActive()){
				//int action = gameInterface->getSelectedAction(x, y);
				//if(action > -1 && action < NUM_ACTION_TYPES)
			dprintf(("  item num: %d\n", gameInterface->actionMenu->getItemAt(x, y)));
			gameInterface->hideActionMenu();
			//}
		} // playing state
		break;		
		
	}	
}

//////////////////////////////////////////////////////////

void RoboPanicGame::mouseMotionHandler(UINT message, WPARAM wparam, LPARAM lparam){
	int x, y;
	x = LOWORD(lparam);	y = HIWORD(lparam);
	//dprintf(("   ----> mouse motion: %d, %d\n", x, y));
}

//////////////////////////////////////////////////////////

bool RoboPanicGame::startMainLoop(){
	MSG msg;
	while(TRUE){
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
			if(!GetMessage(&msg, NULL, 0, 0))	
				return msg.wParam ? true : false; // avoid VC++ warning
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else if(ActiveApp)  composeFrame();
		else WaitMessage();
	}
	return FALSE; // should never get here
}

//////////////////////////////////////////////////////////

void RoboPanicGame::release(){	
	if(titleScreen) titleScreen->release();	
	if(creditScreen) creditScreen->release();	
	if(howPlayScreen) howPlayScreen->release();;
	if(levelScreen) levelScreen->release();
	if(completeScreen) completeScreen->release();	
	if(preGameScreen) preGameScreen->release();
	if(pauseScreen) pauseScreen->release();
	if(startText) startText->release();
	if(continueText) continueText->release();
	if(world) world->release();
	gameInterface->release();
}

//////////////////////////////////////////////////////////

bool RoboPanicGame::restoreSurfaces(){	
	dprintf(("SHITTY"));
	if(lpPrimary->Restore() == DD_OK)
		world->draw(lpPrimary);
	else return FALSE;

	if(lpSecondary->Restore() == DD_OK)
		world->draw(lpSecondary);
	else return FALSE;

	if(titleScreen->restore())
		titleScreen->load("images/title.bmp");
	else return FALSE;

	if(startText->restore())
		startText->load("images/startmtext.bmp");
	else return FALSE;

	if(continueText->restore())
		continueText->load("images/contmtext.bmp");
	else return FALSE;

	if(creditScreen->restore())
		creditScreen->load("images/credits.bmp");
	else return FALSE;

	if(pauseScreen->restore())
		pauseScreen->load("images/pausemenu.bmp");
	else return FALSE;

	if(howPlayScreen->restore())		
		howPlayScreen->load("images/howplay.bmp");
	else return false;
	
	if(levelScreen->restore())
		levelScreen->load("images/level.bmp");
	else return false;
	
	if(completeScreen->restore())
		completeScreen->load("images/complete.bmp");
	else return false;
		
	if(world->restore()){
		world->draw(lpSecondary);
		world->draw(lpPrimary);
	}
	
	if(gameState == GamePlaying)  changeGameState(GamePaused);
	else changeGameState(gameState);

	return TRUE;	
}

