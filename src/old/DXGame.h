//
// DXGame.h: interface for the DXGame class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _DXGAME_H
#define _DXGAME_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define INITGUID
#include <objbase.h>
#include <dinput.h>
	
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dsound.h>
#include "defines.h" 

#define LPDDObject LPDIRECTDRAW
#define LPDDSurface LPDIRECTDRAWSURFACE 
#define LPDDPalette LPDIRECTDRAWPALETTE 
#define DDSurfaceDesc DDSURFACEDESC

extern LPDDObject lpDirectDrawObject;
extern LPDDSurface lpPrimary;
extern LPDDSurface lpSecondary;
extern LPDIRECTINPUT lpDirectInput;
extern LPDIRECTINPUTDEVICE lpKeyboard;

struct D3DDevice;
struct DisplayMode;
struct DisplayDriver;


class DXGame{
public:
	DXGame(int screenWidth, int screenHeight, int colorDepth);
	virtual ~DXGame();
	virtual bool initialize(HINSTANCE hInstance, void* winProc, char *className);	
	virtual bool startMainLoop() = 0;
	void fillSurface(LPDDSurface surface, COLORREF color);
	void drawText(char *text, int x, int y, COLORREF color,  LPDDSurface surface);
	virtual void release();	
	virtual long messageHandler(HWND hwnd, UINT message, WPARAM wParam, 
		LPARAM lParam);
protected:	
	bool changeResolution(int mode);	
	virtual bool pageFlip();  
	virtual bool restoreSurfaces();
	virtual void keyboardHandler(UINT message, WPARAM wParam) = 0;
	virtual void mouseButtonHandler(UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual void mouseMotionHandler(UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool initWindows(HINSTANCE hInstance, void* winProc, char *className);
	virtual bool initDirectX(HINSTANCE hInstance);
	virtual bool initDirectDraw();
	//virtual bool initDirectSound();	
	virtual bool initDirectInput(HINSTANCE hInstance);
	const int screenWidth;
	const int screenHeight;
	const int colorDepth;	
	HWND hwnd;	
	bool ActiveApp;	
	//D3DDevice *d3dDevice;
	DisplayMode *displayModeList;
	DisplayDriver *displayDriverList;
	
};

#endif // _DXGAME_H
