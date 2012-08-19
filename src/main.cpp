//
// main.cpp
//

#include "debugdec.h" //included in main(this file) for debug support
	
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdarg.h>
#include <process.h>

#include "RoboPanicGame.h"

#define WIN32_LEAN_AND_MEAN
#define NODEFAULTLIB

RoboPanicGame *game;
long CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, 
				   int nCmdShow){
	_spawnl(_P_WAIT, ".\\roboshrt.exe", ".\\roboshrt.exe", NULL);	
	game = new RoboPanicGame(SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH);	
	if(!game->initialize(hInstance, windowProc, "RoboPanic")) return FALSE;	
	game->startMainLoop();
	dprintf(("leavin WinMain()"));
	return true;
} //WinMain

//message handler (window procedure)
long CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){	
	return game->messageHandler(hwnd, message, wParam, lParam);
}

