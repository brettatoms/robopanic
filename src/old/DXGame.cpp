//
// DXGame.cpp: implementation of the DXGame class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <unknwn.h> // for QueryInterface
#include <math.h>
#include "DXGame.h"
#include "debug.h"

LPDDObject lpDirectDrawObject = NULL;
LPDDSurface lpPrimary = NULL;
LPDDSurface lpSecondary = NULL ;
LPDDPalette lpPrimaryPalette = NULL;
LPDDPalette lpSecondaryPalette = NULL;
LPDIRECTINPUT lpDirectInput = NULL;
LPDIRECTINPUTDEVICE lpKeyboard = NULL;

void debugCreateSurface(HRESULT);
HRESULT WINAPI enumDisplayModes(LPDDSURFACEDESC pDDSD, LPVOID Context);

BOOL WINAPI enumDisplayDrivers(GUID FAR* guid, LPSTR driverDescription,
                               LPSTR driverName, LPVOID context);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DXGame::DXGame(int screen_width, int screen_height, int color_depth)
	: screenWidth(screen_width), screenHeight(screen_height), colorDepth(color_depth){	
	displayDriverList = NULL;
	displayModeList = NULL;
}


DXGame::~DXGame(){	
	release();		
}

///////////////////////////////////////////////

// member functions


bool DXGame::initialize(HINSTANCE hInstance, void *winProc, char *className){
	dprintf(("entered initialize()\n"));
	if(!initWindows(hInstance, winProc, className)){
		DestroyWindow(hwnd);
		return FALSE;	
	}
	if(!initDirectX(hInstance)){ 
		DestroyWindow(hwnd);
		return FALSE;	
	}
	dprintf(("leaving initialize()\n"));
	return TRUE;
}

///////////////////////////////////////////////

bool DXGame::initWindows(HINSTANCE hInstance, void *winProc, char *className){	
	dprintf(("entered initWindows()\n"));
	WNDCLASS wc;						//window registration info	
	wc.style = CS_HREDRAW | CS_VREDRAW; //style		
	wc.lpfnWndProc	 =	(long (__stdcall *)(HWND, unsigned int, unsigned int, long))winProc;	
	wc.cbClsExtra	 =	wc.cbWndExtra=0;
	wc.hInstance	 =	hInstance;
	wc.hIcon		 =	LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor		 =	LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground =	NULL;
	wc.lpszMenuName  =	className;
	wc.lpszClassName =	className;
	if(!RegisterClass(&wc))  return FALSE;			//register display window	

	//create and set up fullscreen window
	hwnd = CreateWindowEx(WS_EX_TOPMOST, className, className, WS_VISIBLE | WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 
		NULL, NULL, hInstance, NULL);	
	if(!hwnd) return FALSE;	
	
	//set up window	
	UpdateWindow(hwnd);
	SetFocus(hwnd); //allow input from keyboard
	ShowCursor(FALSE); //hide the cursor	
	//dprintf(("leaving initWindows()\n\n"));
	dprintf(("leaving initWindows()\n"));
	return TRUE;
}

///////////////////////////////////////////////

//message handler (window procedure)
long DXGame::messageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){	
	switch(message){
	case WM_ACTIVATEAPP:
		ActiveApp = (wParam ? true : false); // is app active or not		
		break;
    case WM_CREATE:
		dprintf(("window created\n"));
		break;
    case WM_KEYDOWN: case WM_KEYUP:// case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		keyboardHandler(message, wParam); break;	// key hit		
	case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:  // mouse button
	case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		mouseButtonHandler(message, wParam, lParam); break;
	case WM_MOUSEMOVE:		// mouse move
		mouseMotionHandler(message, wParam, lParam); break;		
    case WM_DESTROY:		//end of game, cleanup
		dprintf(("destroying window\n"));		
		ShowCursor(TRUE);	//show the mouse cursor
		PostQuitMessage(0); //and exit
		break;	
    default: //default window procedure		
		return DefWindowProc(hwnd, message, wParam, lParam);
	}// end switch(message)
	return 0L;
} // messageHandler

///////////////////////////////////////////////

bool DXGame::initDirectX(HINSTANCE hInstance){
	if(!initDirectDraw()) return FALSE;
	//if(!initDirectSound()) return FALSE;
	if(!initDirectInput(hInstance)) return FALSE;
	return TRUE;
}

///////////////////////////////////////////////

bool DXGame::changeResolution(int mode){
	return TRUE;
};

///////////////////////////////////////////////

bool DXGame::initDirectDraw(){
	dprintf(("entered initDirectDraw()\n"));	

	//LPDIRECTDRAW temp_lpdd;
	//create direct draw object	  	
	if(DirectDrawCreate(NULL, &lpDirectDrawObject, NULL) != DD_OK){	
	//if(DirectDrawCreate(NULL, &temp_lpdd, NULL) != DD_OK){	
		dprintf(("  ddobject not created\n"));
		DestroyWindow(hwnd); return FALSE;
	}dprintf(("  created direct draw object\n"));	

	/*
	//query interface
	if(temp_lpdd->QueryInterface(IID_IDirectDraw2, (LPVOID*)&lpDirectDrawObject) != S_OK){
		dprintf(("  could not query interfacen"));
		temp_lpdd->Release();
		DestroyWindow(hwnd); return FALSE;
	}
	temp_lpdd->Release();  // just used to get the DD2 interface
*/

	//set cooperative level
	if(lpDirectDrawObject->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK){
		DestroyWindow(hwnd); return FALSE;
	}dprintf(("  coop level set\n"));
	
	//enumerate the display drivers		
	if(DirectDrawEnumerate(enumDisplayDrivers, &displayDriverList) != DD_OK){
		dprintf(("  could not enumerate display driver\n"));
		DestroyWindow(hwnd); return FALSE;
	}

	if(displayDriverList != NULL){
		dprintf(("------------------------------------\n"));

		dprintf(("driver description:\n"));
		if(displayDriverList[0].description != NULL) 
			dprintf(("  %s\n", displayDriverList[0].description));

		//enumerate the display modes
		if(lpDirectDrawObject->EnumDisplayModes(0, NULL, &displayModeList, 
			enumDisplayModes) != DD_OK){			
			dprintf(("  could not enumerate display modes\n"));		
			DestroyWindow(hwnd); return FALSE;
		}

		// make sure the desired display mode is supported
		bool OK = false;
		DisplayMode *dm = NULL;
		for(dm = displayModeList; dm != NULL; dm=dm->next){			
			if(dm->width == screenWidth && dm->height == screenHeight && 
				dm->depth == colorDepth){
				dprintf(("  %dx%d: %d ---> YES\n", dm->width, dm->height, dm->depth));
				OK = true;
			}
		}

		// display an error message box if the display mode is not supported
		if(!OK){
			char message[64];
			sprintf(message, "Could not set display mode: %d x %d: %d", 
				screenWidth, screenHeight, colorDepth);		
			MessageBox(hwnd, message, "Sorry!", MB_OK);
			DestroyWindow(hwnd);
			return false;
		}		
		dprintf(("------------------------------------\n"));

	}// end if displayDriverList != NULL

	// change screen resolution, will not get here if the display mode
	// is not supported
	if(lpDirectDrawObject->SetDisplayMode(screenWidth, screenHeight, colorDepth) !=DD_OK ){		
		dprintf(("  could not set display mode\n"))
		DestroyWindow(hwnd); return FALSE;		
	}dprintf(("  display mode set\n"));

	//create the surfaces
	DDSurfaceDesc ddsd; //direct draw surface descriptor
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;// | DDSD_CKSRCBLT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP 
		| DDSCAPS_COMPLEX;		
	ddsd.dwBackBufferCount = 1;
	//ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = TRANSPARENT_COLOR;
	//ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = TRANSPARENT_COLOR;

	/*
	DDCAPS driverCaps;
	lpDirectDrawObject->GetCaps(&driverCaps, NULL);
	if((driverCaps.dwCaps2 & DDCAPS2_WIDESURFACES) == DDCAPS2_WIDESURFACES){	
		char message[256];
		sprintf(message, "Error: your primary display device does not\n" \
			"support wide surfaces. Check the DirectX setting in the Control \n"
			"Panel and uncheck Disable Wide Surfaces\n");
		dprintf((message));
		MessageBox(hwnd, message, "Sorry!", MB_OK);
		DestroyWindow(hwnd);
		return false;
	}*/

	dprintf(("   passed caps checks\n"));

	HRESULT r;
	if((r = lpDirectDrawObject->CreateSurface(&ddsd, &lpPrimary, NULL)) != DD_OK){
		dprintf(("Error ** could not create primary surface:\n"));
		debugCreateSurface(r);
		return FALSE;
	}	

	//get pointer to the secondary surface
	

	DDSCAPS ddscaps; ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	if(lpPrimary->GetAttachedSurface(&ddscaps, &lpSecondary) != DD_OK){	    
		return false;
	}

	// setup the clipper
	LPDDClipper clipper;
	if(lpDirectDrawObject->CreateClipper(NULL, &clipper, NULL) != DD_OK) return false;
	if(clipper->SetHWnd(NULL, hwnd) != DD_OK) return false;
	if(lpSecondary->SetClipper(clipper) != DD_OK) return false;	
	//clipper->Release();

	dprintf(("entered initDirectDraw()\n"));
	return TRUE;
}

///////////////////////////////////////////////

bool DXGame::initDirectInput(HINSTANCE hInstance){
	dprintf(("Entered initDirectInput\n"));
	if(FAILED(DirectInputCreate(hInstance, DIRECTINPUT_VERSION, &lpDirectInput, NULL)))
		dprintf(("*****COULD NOT CREATE DIRECT INPUT OBJECT*****\n"));
	if(FAILED(lpDirectInput->CreateDevice(GUID_SysKeyboard, &lpKeyboard, NULL)))
		dprintf(("*****COULD NOT CREATE KEYBOARD*****\n"));

	// set cooperation level
	if (lpKeyboard->SetCooperativeLevel(hwnd, 
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK){
		dprintf(("COULD NOT SetCooperative Level\n"));
		return false;
	}
		

	// set data format
	if (lpKeyboard->SetDataFormat(&c_dfDIKeyboard)!=DI_OK){
		dprintf(("COULD NOT Set Data Format\n"));
		return false;
	}
	

	// acquire the keyboard
	if (lpKeyboard->Acquire()!=DI_OK){
		dprintf(("COULD NOT Acquire the keyboard\n"));
		return false;
	}
	
	dprintf(("Leaving the DirectInput\n"));

	return true;
}

///////////////////////////////////////////////

void DXGame::release(){
	if(lpDirectDrawObject != NULL){		//if DD object exists
		if(lpPrimary != NULL)				//if primary surface exists
			lpPrimary->Release();			//release primary surface
		lpDirectDrawObject->Release();	//release DD object
	}	
	if(lpDirectInput != NULL){
		if(lpKeyboard){
			lpKeyboard->Unacquire();
			lpKeyboard->Release();
		} 
		lpDirectInput->Release();
	}
}

///////////////////////////////////////////////

bool DXGame::pageFlip(){ 
  if(lpPrimary->Flip(NULL, DDFLIP_WAIT) == DDERR_SURFACELOST)
    return restoreSurfaces();
  return TRUE;
}

///////////////////////////////////////////////

bool DXGame::restoreSurfaces(){   
	if(lpPrimary->Restore() != DD_OK) return FALSE;    
	if(lpSecondary->Restore() != DD_OK) return FALSE;
	return FALSE;
}

//////////////////////////////////////////////

void DXGame::fillSurface(LPDIRECTDRAWSURFACE lpdds,COLORREF color)
{
	DDBLTFX fx; 
	ZeroMemory(&fx, sizeof(fx));	
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = color; 
	lpdds->Blt(NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&fx);
}


void DXGame::drawText(char *text,int x,int y,COLORREF color,LPDDSurface surface){
	HDC xdc; 
	if (FAILED(surface->GetDC(&xdc)))
		dprintf(("DXGame::drawText\n"));
	SetTextColor(xdc, color);
	SetBkMode(xdc, TRANSPARENT_COLOR);
	TextOut(xdc,x,y,text,strlen(text));
	surface->ReleaseDC(xdc);
}	

