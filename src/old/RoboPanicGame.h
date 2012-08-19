// RoboPanicGame.h: interface for the RoboPanicGame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ROBO_PANIC_GAME_H
#define _ROBO_PANIC_GAME_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DXGame.h"

// in DXGame.cpp
extern LPDDObject lpDirectDrawObject;
extern LPDDSurface lpPrimary;
extern LPDDSurface lpSecondary;
extern LPDDPalette lpPrimaryPalette;
extern LPDDPalette lpSecondaryPalette;

// forward declarations prevents headers in headers
class World;
class Interface;
class CSoundManager;
class BmpLoader;

typedef enum GameState{StartMenu, ContinueMenu, LevelScreen, HowScreen, CreditScreen, 
					   GameChangingScene, GamePaused, GamePlaying, GameComplete};
/*
typedef enum GameState{GameIntro, OptionScreen, CreditScreen, GameMenu, 
					   GameChangingScene, GamePaused, GamePlaying};
					   */

class RoboPanicGame : public DXGame {
public:
	RoboPanicGame(int screenWidth, int screenHeight, int colorDepth);
	virtual ~RoboPanicGame();
	virtual bool startMainLoop();		
	void release();
	bool initialize(HINSTANCE hInstance, void* winProc, char *className);
	Interface *gameInterface;
	void changeGameState(GameState gameState);
protected:	
	virtual bool restoreSurfaces();
	virtual void keyboardHandler(UINT message, WPARAM wparam);
	virtual void directInputHandler();
	virtual void mouseButtonHandler(UINT message, WPARAM wparam, LPARAM lParam);
	virtual void mouseMotionHandler(UINT message, WPARAM wparam, LPARAM lparam);
	
	void scrollInNewScene(LPDDSurface surface);
	void composeFrame();
	//void loadSounds(int level=0);
	void initSounds();
	//CSoundManager *soundManager;
	World *world;
	GameState gameState,prevGameState;	
	long lastComposeTime;
	int waitTime, framesPerSecond;
	BmpLoader* titleScreen;
	BmpLoader* pauseScreen;	
	BmpLoader* howPlayScreen;
	BmpLoader* creditScreen;
	BmpLoader* preGameScreen;
	BmpLoader* levelScreen;
	BmpLoader* completeScreen;	
	BmpLoader* startText;
	BmpLoader* continueText;
};

#endif // _ROBO_PANIC_GAME_H
