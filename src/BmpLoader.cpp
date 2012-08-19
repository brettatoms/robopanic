// BmpLoader.cpp: implementation of the BmpLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "BmpLoader.h"
#include <ddraw.h>
#include "BoundingBox.h"
#include "debug.h"

void debugBlt(HRESULT r);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern LPDirectDraw lpDirectDrawObject;

BmpLoader::BmpLoader(){
	width = height = 0;
	image = NULL;	
}

BmpLoader::~BmpLoader(){
	release();
	image = NULL;
	delete[] m_cImage;
}

///////////////////////////////////////////////////

void BmpLoader::release(){
	if(image) image->Release();
}

///////////////////////////////////////////////////

bool BmpLoader::load(char *filename){	
	HANDLE hfile;		//input file handle
	DWORD actualRead;	//number of bytes actually read	
	BOOL OK=TRUE;		//no error has occurred yet

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
	
	width = bmpFileInfo.biWidth;
	height = bmpFileInfo.biHeight;
	if(!createSurface()) return false;
	else return load(image, filename);
}

///////////////////////////////////////////////////

// taken from Inside DirectX, Miscrosoft Press 199(?)
// load a .bmp file of size width x height and blit it to this->surface
bool BmpLoader::load(char *filename, int width, int height){
	this->width = width;
	this->height = height;
	if(!createSurface()) return FALSE;
	return load(this->image, filename);	
}

///////////////////////////////////////////////////

// taken from Inside DirectX, Miscrosoft Press 199(?)
// load a .bmp file and draw it to <surface>, dependent on size of
// <surface>, will scale the image to fit to surface
bool BmpLoader::load(LPDDSurface lpDDS, char *filename){
    HBITMAP         hbm;
    HDC             hdcImage = NULL;
    HDC             hdcSurf  = NULL;
    BOOL            bReturn  = FALSE;
    DDSURFACEDESC   ddsd;

    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);

    if(FAILED(lpDDS->GetSurfaceDesc(&ddsd))){
        goto Exit;
    }

	strcpy(this->filename,filename);

    // If the pixel format isn't some flavor of RGB, 
	// we can't handle it.
    if((ddsd.ddpfPixelFormat.dwFlags != DDPF_RGB) ||
         (ddsd.ddpfPixelFormat.dwRGBBitCount < 16))    {
        dprintf(("Nonpalettized RGB mode required.\n"));
        goto Exit;        
    }

    // Try loading the image.
    hbm = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, ddsd.dwWidth, 
            ddsd.dwHeight, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    if(hbm == NULL) 	{
        dprintf(("Couldn't find the resource.\n" ));
        goto Exit;
    }

    // Create a DC and select the image into it.
    hdcImage = CreateCompatibleDC(NULL);
    SelectObject(hdcImage, hbm);
   
    // Get a DC for the surface.
    if(FAILED(lpDDS->GetDC(&hdcSurf))){
        dprintf(( "Couldn't get a DC.\n"));
        goto Exit;
    }
    
    // The BitBlt will perform format conversion as necessary.
    if(BitBlt(hdcSurf, 0, 0, ddsd.dwWidth, ddsd.dwHeight, 
                 hdcImage, 0, 0, SRCCOPY) == FALSE) {
        dprintf(("Blt failed.\n" ));
        goto Exit;
    }

    // Success.
    bReturn = TRUE;
    
Exit:
    // Clean up everything.
    if(hdcSurf) lpDDS->ReleaseDC(hdcSurf);
    if(hdcImage) DeleteDC(hdcImage);
    if(hbm) DeleteObject(hbm);

	return (bReturn ? TRUE : FALSE);	
}

///////////////////////////////////////////////////

bool BmpLoader::drawBox(LPDDSurface surface, BoundingBox *destBox, BoundingBox *srcBox,
						DWORD bltFlags){
	HRESULT r;		
	if(srcBox == NULL && destBox == NULL)
		r = surface->Blt(NULL, image, NULL, bltFlags, NULL);
	else if(destBox == NULL)
		r = surface->Blt(NULL, image, srcBox->toRECT(), bltFlags, NULL);
	else if(srcBox == NULL)
		r = surface->Blt(destBox->toRECT(), image, NULL, bltFlags, NULL);
	else r = surface->Blt(destBox->toRECT(), image, srcBox->toRECT(), 
		bltFlags, NULL);	
	return r == DD_OK ? true : false;
}

///////////////////////////////////////////////////

bool BmpLoader::draw(LPDDSurface surface, int destX, int destY, DWORD bltFlags){
	RECT destRect;
	destRect.left = destX;
	destRect.top = destY;
	destRect.right= destX + width;
	destRect.bottom= destY + height+1;
	HRESULT r;				
	r = surface->Blt(&destRect, image, NULL, bltFlags, NULL);		
	return ((r == DD_OK) ? TRUE : FALSE);
}

///////////////////////////////////////////////////

// draw some rectangle of this->image to 0,0 on surface
bool BmpLoader::drawRect(LPDDSurface surface, int srcX, int srcY, 
						 int srcWidth, int srcHeight){
	if(image == NULL) return FALSE;
	return drawRectTo(0, 0, surface, srcX, srcY, srcWidth, srcHeight);
}

///////////////////////////////////////////////////

// draw a rectagle from this->image to destX, destY on the dest surface
bool BmpLoader::drawRectTo(int destX, int destY, LPDDSurface surface, int srcX, 
						 int srcY, int srcWidth, int srcHeight){
	//dprintf(("entered BmpLoader::drawRectTo\n"));	
	if(image == NULL) return FALSE;
	RECT srcRect;
	srcRect.left = srcX;
	srcRect.top = srcY;
	srcRect.right = srcX + srcWidth;
	srcRect.bottom = srcY + srcHeight;

	RECT destRect;
	destRect.left = destX;
	destRect.top = destY;
	destRect.right= destX + srcWidth;
	destRect.bottom= destY + srcHeight;
	
	HRESULT r;		
	r = surface->Blt(&destRect, image, &srcRect, DDBLT_WAIT, NULL);	
	return ((r == DD_OK) ? TRUE : FALSE);
}

///////////////////////////////////////////////////

bool BmpLoader::createSurface(){
	DDSurfaceDesc ddsd; ZeroMemory(&ddsd, sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);							 //required field
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_CKSRCBLT;
	//ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;		//offscreen surface
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	
	ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = TRANSPARENT_COLOR;
	ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = TRANSPARENT_COLOR;
	

	HRESULT result;
	result = lpDirectDrawObject->CreateSurface(&ddsd, &image, NULL);	
	return ((result == DD_OK) ? TRUE : FALSE);
}

///////////////////////////////////////////////////

long BmpLoader::getHeight(){ return height; }
long BmpLoader::getWidth(){ return width; }

///////////////////////////////////////////////////

bool BmpLoader::surfaceLost(){
	dprintf(("Entering the BmpLoader::surfaceLost\n"));
	if(image->IsLost() == DDERR_SURFACELOST){
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////

bool BmpLoader::restore(){
	dprintf(("Entering the BmpLoader::restore\n"));	
	image->Restore();	
	//dprintf(("restored image: %s\n", filename));
	dprintf(("Leaving the BmpLoader::restore\n"));
	return load(image, filename);
	//return true;
}