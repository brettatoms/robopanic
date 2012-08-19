//Sprite.cpp

#include "Sprite.h"

extern LPDirectDraw lpDirectDrawObject;

void debugBlt(HRESULT);


Sprite::Sprite(char *filename, int frames, int w, int h, int rows, int columns){
	bitmapWidth=bitmapHeight=0;
	spriteWidth=w;
	spriteHeight=h;
	image=NULL;
	sprite=NULL;
	maxFrames=frames;
	numberRows=rows;
	numberColumns=columns;
	load(filename);
}



Sprite::~Sprite(){
	for(int i=0; i<maxFrames; i++)
		sprite[i]->Release();
	sprite=NULL;
}



bool Sprite::load(char *filename){	
	HANDLE hfile;		//input file handle
	DWORD actualRead;	//number of bytes actually read	
	BOOL OK=TRUE;		//no error has occurred yet
	BITMAPFILEHEADER bmpFileHead;
	BITMAPINFOHEADER bmpFileInfo; 

	//open input file for reading
	hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 
		(HANDLE)NULL);  
	if(hfile == INVALID_HANDLE_VALUE)  return FALSE;

	//read the header and info structures
	OK = ReadFile(hfile, &bmpFileHead, sizeof(bmpFileHead), &actualRead, NULL);
	if(OK) 
		OK = ReadFile(hfile, &bmpFileInfo, sizeof(bmpFileInfo), &actualRead, NULL);

	CloseHandle(hfile);
	if(!OK) return FALSE;
	
	bitmapWidth = bmpFileInfo.biWidth;
	bitmapHeight = bmpFileInfo.biHeight;
	if(!createSurface()) return false;
	else return load(image, filename);
}


/*
bool Sprite::load(char *filename, int width, int height){
	bitmapWidth = width;
	bitmapHeight = height;
	if(!createSurface()) return FALSE;
	return load(this->image, filename);	
}
*/


bool Sprite::load(LPDIRECTDRAWSURFACE lpDDS, LPSTR szImage){
    HBITMAP         hbm;
    HDC             hdcImage = NULL;
    HDC             hdcSurf  = NULL;
    BOOL            bReturn  = FALSE;
    DDSURFACEDESC   ddsd;
dprintf(("************GETTING INTO THE LOAD\n"));
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize = sizeof( ddsd );

    if ( FAILED( lpDDS->GetSurfaceDesc( &ddsd ) ) )	{
        goto Exit;
    }

    // If the pixel format isn't some flavor of RGB, 
	// we can't handle it.
    if ( ( ddsd.ddpfPixelFormat.dwFlags != DDPF_RGB ) ||
         ( ddsd.ddpfPixelFormat.dwRGBBitCount < 16 ) )    {
        OutputDebugString( "Nonpalettized RGB mode required.\n" );
        goto Exit;        
    }


    // Try loading the image.
    hbm = ( HBITMAP )LoadImage( NULL, szImage, 
            IMAGE_BITMAP, ddsd.dwWidth, 
            ddsd.dwHeight, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    if ( hbm == NULL ) 	{
        OutputDebugString("Couldn't find the resource.\n" );
        goto Exit;
    }

	dprintf(("*****************LOADED THE BITMAP\n"));
    // Create a DC and select the image into it.
    hdcImage = CreateCompatibleDC( NULL );
    SelectObject( hdcImage, hbm );
   
    // Get a DC for the surface.
    if ( FAILED( lpDDS->GetDC( &hdcSurf ) ) )	{
        OutputDebugString( "Couldn't get a DC.\n");
		dprintf(("COULDNT GET A DC"));
        goto Exit;
    }
    
    // The BitBlt will perform format conversion as necessary.
    if ( BitBlt( hdcSurf, 0, 0, ddsd.dwWidth, ddsd.dwHeight, 
                 hdcImage, 0, 0, SRCCOPY ) == FALSE ) 	{
        OutputDebugString( "Blt failed.\n" );
		dprintf(( "Blt failed.\n" ));
        goto Exit;
    }
dprintf(("************EXITING THE LOAD FUNCTION\n"));
    // Success.
    bReturn = TRUE;
    
Exit:
    // Clean up everything.
    if ( hdcSurf ) lpDDS->ReleaseDC( hdcSurf );
    if ( hdcImage ) DeleteDC( hdcImage );
    if ( hbm ) DeleteObject( hbm );

	return (bReturn ? TRUE : FALSE);	
	
}


bool Sprite::createSurface(){
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);							 //required field
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; //attributes needed		
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;//offscreen surface
	ddsd.dwWidth = bitmapWidth;
	ddsd.dwHeight = bitmapHeight;

	DDCOLORKEY ddck; //direct draw color descriptor
    ddck.dwColorSpaceLowValue=TRANSPARENT_COLOR;
	ddck.dwColorSpaceHighValue=TRANSPARENT_COLOR; //one color

	HRESULT result;
	result = lpDirectDrawObject->CreateSurface(&ddsd, &image, NULL);
	image->SetColorKey(DDCKEY_SRCBLT, &ddck);
	return ((result == DD_OK) ? TRUE : FALSE);
}


bool Sprite::draw(LPDIRECTDRAWSURFACE surface, int x, int y, int index){		
	RECT dest,src;
	dest.left = x; 	dest.top = y;
	dest.right = x + spriteWidth-1; 	dest.bottom = y + spriteHeight-1;

	src.left=0;	src.top=0;
	src.right=spriteWidth-1; src.bottom=spriteHeight-1;

	//dprintf(("About to Blt the sprite to the screen"));
	if(surface->Blt(&dest, sprite[index], &src, DDBLT_KEYSRC|DDBLT_WAIT, NULL)!=DD_OK){
		dprintf(("Failed to Blt the sprite to the screen\n"));
		return FALSE;
	}
	
	return TRUE;
}


bool Sprite::loadSprites(){
dprintf(("Entering Sprite::loadSprites\n"));
	RECT dest;
	dest.left=0; dest.top=0;
	dest.right=spriteWidth-1; dest.bottom=spriteHeight-1;

	//allocate memory for the number of sprites
	sprite = new LPDIRECTDRAWSURFACE[numberRows*numberColumns];

	//create Direct Draw Surface Descriptor
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	//memset(&ddsd, 0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);							 //required field
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; //attributes needed		
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;//offscreen surface
	ddsd.dwWidth = spriteWidth;
	ddsd.dwHeight = spriteHeight;

	DDCOLORKEY ddck; //direct draw color descriptor
	ddck.dwColorSpaceLowValue=
	ddck.dwColorSpaceHighValue=TRANSPARENT_COLOR; //one color

	int w=spriteWidth;
	int h=spriteHeight;
	RECT src;
	int count=0;
	for (int i=0; i<numberRows; i++){
		for (int j=0; j<numberColumns; j++){
			src.left=(j*w)+((j+1)*1); src.right=src.left+w-1;
			src.top=(i*h)+((i+1)*1);  src.bottom=src.top+h-1;
			if(lpDirectDrawObject->CreateSurface(&ddsd, &sprite[count], NULL))
				dprintf(("***COULD NOT CREATE OBJECT\n"));
			sprite[count]->SetColorKey(DDCKEY_SRCBLT, &ddck);
			if(sprite[count]->Blt(&dest, image, &src, DDBLT_WAIT, NULL)!=DD_OK)
				dprintf(("COULD NOT BLT\n"));
			count++;
		}
	}


	if(sprite[0]==NULL){
		dprintf(("Problem creating the surfaces"));
	}
	else
		dprintf(("No problem creating the surfaces"));
	//need to put this in the for loop and change the 
	//top, bottom and sides to build the sprite from the
	//bitmap image
	//RECT src;
	//src.left=1; src.top=1;
	//src.right=spriteWidth; src.bottom=spriteHeight;
	image->Release();
	image=NULL;

	dprintf(("Leaving Sprite::loadSprites\n"));
	return TRUE;
}

bool Sprite::surfaceLost(){
	bool lost = false;
	for(int i=0; i<maxFrames; i++){
		if(sprite[i]->IsLost() == DDERR_SURFACELOST){
			lost = true;
		}
	}
	return lost;
}

bool Sprite::restore(){
	for(int i=0; i<maxFrames; i++){
		if(sprite[i]->IsLost() == DDERR_SURFACELOST)
			sprite[i]->Restore();
	}
	return true;
}

void Sprite::release(){
	
}