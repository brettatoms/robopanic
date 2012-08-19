// BmpLoader.h: interface for the BmpLoader class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BMPLOADER_H_
#define _BMPLOADER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <ddraw.h>
#include "wingdi.h"
#include "defines.h"

class BoundingBox;

class BmpLoader{
public:
	BmpLoader();
	virtual ~BmpLoader();
	bool surfaceLost();
	bool load(char *filename);    // queries the file for the image size
	bool load(LPDDSurface surface, char *filename);
	bool load(char *filename, int width, int height);
	//bool drawBox(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox);
	bool drawBox(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox, 
		DWORD bltFlags = DDBLT_WAIT);
	bool draw(LPDDSurface surface, int destX, int destY, DWORD bltFlags = DDBLT_WAIT);
	bool drawRect(LPDDSurface destSurface, int srcX, int srcY, int srcWidth, 
		int srcHeight);
	bool drawRectTo(int destX, int destY, LPDDSurface destSurface, int srcX, 
		int srcY, int srcWidth, int srcHeight);
	long getHeight();
	long getWidth();		
	void release();
	bool restore();
protected:
	char filename[64];
	BITMAPFILEHEADER bmpFileHead; //bmp header
    BITMAPINFOHEADER bmpFileInfo; //bmp file information	
	BYTE *m_cImage; //the image
	LPDDSurface image;
	bool createSurface();
	long width, height;
};

#endif // _BMPLOADER_H_
