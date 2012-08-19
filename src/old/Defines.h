//defines.h: essential defines

#ifndef _DEFINES_H
#define _DEFINES_H

#include "windows.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SCREEN_WIDTH 640	//pixels wide
#define SCREEN_HEIGHT 480	//pixels high
#define COLOR_DEPTH 16		//number of bits to store colors
#define TRANSPARENT_COLOR	RGB(255, 255, 255)


#define LPDirectDraw		LPDIRECTDRAW
#define LPDDSurface			LPDIRECTDRAWSURFACE
#define LPDDPalette			LPDIRECTDRAWPALETTE
#define LPDDClipper			LPDIRECTDRAWCLIPPER
#define LPLPDSoundBuffer	LPLPDIRECTSOUNDBUFFER 
#define LPDSoundBuffer		LPDIRECTSOUNDBUFFER 
#define DDSCaps				DDSCAPS
#define DDSurfaceDesc		DDSURFACEDESC



#define FC __fastcall 

typedef enum{ACTION_LOOKAT = 0, ACTION_PICKUP,  ACTION_USE_ITEM_ON, ACTION_PUSH, 
	ACTION_PULL, NUM_ACTION_TYPES} ActionType;

typedef enum{TRIGGER_OPENED=0, TRIGGER_CLOSED, TRIGGER_PUZZLE_COMPLETE,
	NUM_TRIGGER_TYPES} TriggerType;

typedef enum {TILE_WALKTHRU=0,  /*TILE_PICKUPABLE,*/ TILE_SWITCH, TILE_ONTOP,
	NUM_TILE_ATTRIBS} TileAttribute;

typedef enum{ENTITY_PICKUPABLE=0, ENTITY_MOVEABLE, ENTITY_WALKTHRU, 
	NUM_ENTITY_ATTRIBS} EntityAttributes;

typedef enum{NORTH=0, EAST, SOUTH, WEST, NUM_DIRECTIONS} Direction;

typedef enum{ALIVE, WALKING, HIT, RECOVERING, SHOOTING, DYING, DEAD} State;

typedef enum{GUN_SOUND=0, PRE_SOUND, INTRO_SOUND, PANIC_SOUND, CHANGE_SCENE_SOUND, MENU_SOUND, 
	SCENE_BG_SOUND, PAUSED_SOUND, COMPLETE_SOUND, HIT_CHAR_SOUND, HIT_PLAYER_SOUND, NUM_SOUNDS} SoundType;
/*
typedef enum{GUN_SOUND=0, INTRO_SOUND, CHANGE_SCENE_SOUND, MENU_SOUND, 
	SCENE_BG_SOUND, PAUSED_SOUND, NUM_SOUNDS} SoundType;
	*/

typedef struct{
	int type;	
	bool attributes[NUM_TILE_ATTRIBS];
	
	//char level; // not used
	//long nextLevel; // not used
	//int row, col;  // not stored in file
} SceneFileTileData;

typedef struct{
    char engName[16];   //Map engine name.
    int engVersion;     //Map engine version.
    char sceneName[16];   //Map short name.
    char sceneDesc[128];  //Map long description.
    char tileSet[32];   //Name of tileset used for map.
    long tileRows;      //Width of the map.
    long tileCols;     //Height of the map.
} SceneFileHdr;

struct D3DDevice;
struct DisplayMode;
struct DisplayDriver;

struct D3DDevice{
   D3DDevice* next;
   GUID guid;
   char* name;
   bool isHW;
};

struct DisplayMode{
   DisplayMode* next;
   int width;
   int height;
   int depth;
};

struct DisplayDriver{
	DisplayDriver* next;
	GUID guid;
	char* description;	
	D3DDevice* d3dDeviceList;
	DisplayMode* displayModeList;
};

//typedef void (*)(Entity *source, int TriggerType) TriggerCallback 


//enum EventType{LEFT_MOUSE_CLICK=0, RIGHT_MOUSE_CLICK, NUM_EVENT_TYPES};

#endif // _DEFINES_H
