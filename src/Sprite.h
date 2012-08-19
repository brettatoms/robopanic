//sprite.h

#ifndef _SPRITE_
#define _SPRITE_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <ddraw.h>
#include "debug.h"
#include "defines.h"

class Sprite{
	public:
		Sprite(char *filename, int frames, int w, int h, int rows, int columns);
		~Sprite();
		bool draw(LPDIRECTDRAWSURFACE surface, int x, int y, int index);
		bool load(char *filename);
		bool loadSprites();
		int width(){ return spriteWidth; };
		int height(){ return spriteHeight; };		
		bool restore();
		bool surfaceLost();
		void release();
	protected:
		bool createSurface();
		bool load(LPDIRECTDRAWSURFACE lpDDS, LPSTR szImage);
		LPDIRECTDRAWSURFACE image;
		LPDIRECTDRAWSURFACE *sprite;
		int bitmapWidth, bitmapHeight;
		int maxFrames;
		int spriteWidth, spriteHeight;
		int numberRows, numberColumns;

};

#endif _SPRITE_