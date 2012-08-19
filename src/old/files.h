#ifndef _FILES_H
#define _FILES_H

// standard filenames and defs for the bitmaps
#define TILES_FILE_NAME			"images\\tiles.bmp"
#define TILES_FILE_ROWS			14
#define TILES_FILE_COLUMNS		19
#define TILES_FILE_HEIGHT		463
#define TILES_FILE_WIDTH		628
// for 64x64 tiles
/*
#define TILES_FILE_COLUMNS		10
#define TILES_FILE_ROWS			15
#define TILES_FILE_HEIGHT		496
#define TILES_FILE_WIDTH		651
*/

#define ACTION_BUTTON_FILE_NAME "images\\actionbuttons.bmp" 
#define ACTION_BUTTON_WIDTH		48
#define ACTION_BUTTON_HEIGHT	48
#define ACTION_BUTTON_FILE_COLUMNS	5
#define ACTION_BUTTON_FILE_ROWS		5

#define CHAR_FILE_NAME		"images\\character.bmp"
#define CHAR_FRAMES			36
#define CHAR_WIDTH			58
#define CHAR_HEIGHT			63
/*
#define CHAR_WIDTH			64
#define CHAR_HEIGHT			70
*/
#define CHAR_ROWS			4
#define CHAR_COLUMNS		9


#define ENEMY1_FILE_NAME	"images\\alien3.bmp"
#define ENEMY1_FRAMES		36
#define ENEMY1_WIDTH		58
#define ENEMY1_HEIGHT		63
#define ENEMY1_ROWS			4
#define ENEMY1_COLUMNS		9

#define ENEMY2_FILE_NAME	"images\\skel.bmp"
#define ENEMY2_FRAMES		5
#define ENEMY2_WIDTH		57
#define ENEMY2_HEIGHT		72
#define ENEMY2_ROWS			1
#define ENEMY2_COLUMNS		4

#define BULLET_FILE			"images\\shot.bmp"
#define BULLET_FRAMES		1
#define BULLET_WIDTH		39
#define BULLET_HEIGHT		39
#define BULLET_ROWS			1
#define BULLET_COLUMNS		1

#define EBULLET_FILE			"images\\shot.bmp"
#define EBULLET_FRAMES		1
#define EBULLET_WIDTH		39
#define EBULLET_HEIGHT		39
#define EBULLET_ROWS		1
#define EBULLET_COLUMNS		1

//#define DOOR_FRAMES		5

#define H_DOOR_FILE_NAME	"images\\h_door.bmp"
#define H_DOOR_FRAMES		5
#define	H_DOOR_WIDTH		64
#define	H_DOOR_HEIGHT		20
#define	H_DOOR_ROWS			5
#define	H_DOOR_COLS			1

#define V_DOOR_FILE_NAME	"images\\v_door.bmp"
#define V_DOOR_FRAMES		5
#define	V_DOOR_WIDTH		20
#define	V_DOOR_HEIGHT		96
#define	V_DOOR_ROWS			1
#define	V_DOOR_COLS			5

#define HEALTH_FILE_NAME	"images\\health.bmp"
#define	HEALTH_FILE_WIDTH	24
#define	HEALTH_FILE_HEIGHT	24
#define	HEALTH_WIDTH		24
#define	HEALTH_HEIGHT		24


static char *SceneDataFileNames[] = 	
	{"data\\scene1.rpd",
	 "data\\scene2.rpd",
	 "data\\scene3.rpd",
	 "data\\scene4.rpd",
	 "data\\scene5.rpd",
	 "data\\scene6.rpd",
	 "data\\scene7.rpd"
	};

//standard filenames for the audion files


#endif // _FILES_H