#include <ddraw.h>
#include "debug.h"
#include "defines.h"

extern LPDirectDraw lpDirectDrawObject;
//#define LPDDSurface LPDIRECTDRAWSURFACE

struct D3DDevice;
struct DisplayMode;
struct DisplayDriver;

/*
struct D3DDevice{
   D3DDevice* next;
   GUID guid;
   char* name;
   BOOL isHW;
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
*/

void clearSurface(LPDDSurface surface){
	DDBLTFX fx; ZeroMemory(&fx, sizeof(fx));	
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = RGB(0,0,0);
	surface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);	
}


bool createOffScreenSurface(LPDDSurface &surface, int width, int height){
	DDSurfaceDesc ddsd;  ZeroMemory(&ddsd, sizeof(ddsd));	
	ddsd.dwSize = sizeof(ddsd);							 //required field
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_CKSRCBLT;
	//ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;//offscreen surface
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = TRANSPARENT_COLOR;
	ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = TRANSPARENT_COLOR;

	HRESULT result;
	result = lpDirectDrawObject->CreateSurface(&ddsd, &surface, NULL);	
	return ((result == DD_OK) ? true : false);
}

///////////////////////////////////////////////

//BOOL WINAPI DXGame::enumDisplayModes(LPDDSURFACEDESC pDDSD, LPVOID Context){
HRESULT WINAPI enumDisplayModes(LPDDSURFACEDESC pDDSD, LPVOID Context){
   DisplayMode **DMList = (DisplayMode**)Context;

   DisplayMode* NewDM = new DisplayMode;
   if(NewDM){
      NewDM->width	= pDDSD->dwWidth;
      NewDM->height	= pDDSD->dwHeight;
      NewDM->depth	= pDDSD->ddpfPixelFormat.dwRGBBitCount;
      NewDM->next = NULL;
   }

   if(!(*DMList))
      *DMList = NewDM;
   else{
      for( DisplayMode* TheDM = *DMList; TheDM->next; TheDM = TheDM->next )  
		  ;
      TheDM->next = NewDM;
   }
	
   return DDENUMRET_OK;
   //return true;
}

///////////////////////////////////////////////

//BOOL WINAPI DXGame::enumDisplayDrivers(GUID FAR* guid, LPSTR driverDescription,
BOOL WINAPI enumDisplayDrivers(GUID FAR* guid, LPSTR driverDescription,
							   //LPSTR driverName, LPVOID context){
							   LPSTR driverName, LPVOID displayDriverList){
   DisplayDriver* NewDD = new DisplayDriver;
   if(NewDD){
      ZeroMemory(NewDD, sizeof(DisplayDriver));
      if(guid) CopyMemory(&(NewDD->guid), guid, sizeof(GUID));

      NewDD->description = new char[strlen(driverDescription) + 1];
      if( NewDD->description ) strcpy( NewDD->description, driverDescription );      NewDD->displayModeList = NULL;
      NewDD->d3dDeviceList = NULL;
      NewDD->next = NULL;
   }

   if(!displayDriverList) displayDriverList = NewDD;
   else{
      for(DisplayDriver* TheDD = (DisplayDriver *)displayDriverList; TheDD->next; TheDD = TheDD->next )
         ;

      TheDD->next = NewDD;
   }
	
   //return DDENUMRET_OK;
   return true;
}


/*
HRESULT CALLBACK EnumDisplayDrivers( GUID FAR* pGuid, LPSTR DriverDescription,
                                     LPSTR DriverName, LPVOID Context,
                                     HMONITOR HMonitor )
{
   DisplayDriver* NewDD = new DisplayDriver;
   if( NewDD )
   {
      ZeroMemory( NewDD, sizeof( DisplayDriver ));

      if( pGuid )
         CopyMemory( &( NewDD->Guid ), pGuid, sizeof( GUID ));

      NewDD->Description = new char[strlen( DriverDescription ) + 1];
      if( NewDD->Description )
      {
         strcpy( NewDD->Description, DriverDescription );
      }

      NewDD->DisplayModeList = NULL;
      NewDD->D3DDeviceList = NULL;
      NewDD->Next = NULL;
   }

   if( !DisplayDriverList )
      DisplayDriverList = NewDD;
   else
   {
      for( DisplayDriver* TheDD = DisplayDriverList; TheDD->Next; TheDD = TheDD->Next )
         ;

      TheDD->Next = NewDD;
   }
	
   return DDENUMRET_OK;
}
*/


#ifdef DEBUG_ON

void debugCreateSurface(HRESULT result){
	switch(result){
	case DD_OK: 
		dprintf(("  DD_OK ** hot damn\n"));
		break;
	case DDERR_INCOMPATIBLEPRIMARY:
		dprintf(("  DDERR_INCOMPATIBLEPRIMARY\n\n"));
		break;
	case DDERR_INVALIDCAPS:
		dprintf(("  DDERR_INVALIDCAPS \n\n"));
		break;
	case DDERR_INVALIDOBJECT:
		dprintf(("  DDERR_INVALIDOBJECT\n\n"));
		break;
	case DDERR_INVALIDPARAMS:
		dprintf(("  DDERR_INVALIDPARAMS\n"));
		break;
	case DDERR_INVALIDPIXELFORMAT :
		dprintf(("  DDERR_INVALIDPIXELFORMAT\n"));
		break;
	case DDERR_NOALPHAHW :
		dprintf(("  DDDDERR_NOALPHAHW_OK\n"));
		break;
	case DDERR_NOCOOPERATIVELEVELSET :
		dprintf(("  DDERR_NOCOOPERATIVELEVELSET\n"));
		break;
	case DDERR_NODIRECTDRAWHW :
		dprintf(("  DDERR_NODIRECTDRAWHW\n"));
		break;
	case DDERR_NOEMULATION :
		dprintf(("  DDERR_NOEMULATION\n"));
		break;
	case DDERR_NOEXCLUSIVEMODE :
		dprintf(("  DDERR_NOEXCLUSIVEMODE\n"));
		break;
	case DDERR_NOFLIPHW :
		dprintf(("  DDERR_NOFLIPHW\n"));
		break;
	case DDERR_NOMIPMAPHW :
		dprintf(("  DDERR_NOMIPMAPHW\n"));
		break;
	case DDERR_NOZBUFFERHW :
		dprintf(("  DDERR_NOZBUFFERHW\n"));
		break;
	case DDERR_OUTOFMEMORY :
		dprintf(("  DDERR_OUTOFMEMORY\n"));
		break;
	case DDERR_OUTOFVIDEOMEMORY :
		dprintf(("  DDERR_OUTOFVIDEOMEMORY\n"));
		break;
	case DDERR_PRIMARYSURFACEALREADYEXISTS :
		dprintf(("  DDERR_PRIMARYSURFACEALREADYEXISTS\n"));
		break;
	case DDERR_UNSUPPORTEDMODE :
		dprintf(("  DDERR_UNSUPPORTEDMODE\n"));
		break;
	default: dprintf(("  ** debugCreateSurface(): don't know\n\n"));
		break;
	}
}

void debugBltFast(HRESULT r){
	switch(r){
	case DD_OK: dprintf((" ** DD_OK\n\n")); break;
	case DDERR_EXCEPTION: dprintf((" DDERR_EXCEPTION\n\n")); break;
	case DDERR_GENERIC: dprintf((" ** DDERR_GENERIC\n")); break;
	case DDERR_INVALIDOBJECT: dprintf((" ** DDERR_INVALIDOBJECT\n")); break;
	case DDERR_INVALIDPARAMS: dprintf((" ** DDERR_INVALIDPARAMS\n")); break;
	case DDERR_INVALIDRECT: dprintf((" ** DDERR_INVALIDRECT\n")); break;
	case DDERR_NOBLTHW : dprintf((" ** DDERR_NOBLTHW\n")); break;
	case DDERR_SURFACEBUSY: dprintf((" ** DDERR_SURFACEBUSY\n")); break;
	case DDERR_SURFACELOST: dprintf((" ** DDERR_SURFACELOST\n")); break;
	case DDERR_UNSUPPORTED: dprintf((" ** DDERR_UNSUPPORTED\n")); break;
	default: dprintf((" ** debugBltFast: don't know\n")); break;
	}
}

void debugBlt(HRESULT r){
	switch(r){
	case DD_OK: dprintf((" ** DD_OK\n\n")); break;
	case DDERR_GENERIC: dprintf((" ** DDERR_GENERIC\n")); break;
	case DDERR_INVALIDCLIPLIST: dprintf((" DDERR_INVALIDCLIPLIST\n\n")); break;
	case DDERR_INVALIDOBJECT: dprintf((" ** DDERR_INVALIDOBJECT\n")); break;
	case DDERR_INVALIDPARAMS: dprintf((" ** DDERR_INVALIDPARAMS\n")); break;
	case DDERR_INVALIDRECT: dprintf((" ** DDERR_INVALIDRECT\n")); break;
	case DDERR_NOALPHAHW: dprintf((" ** DDERR_NOALPHAHW\n")); break;
	case DDERR_NOBLTHW : dprintf((" ** DDERR_NOBLTHW\n")); break;
	case DDERR_NOCLIPLIST: dprintf((" ** DDERR_NOCLIPLIST\n")); break;
	case DDERR_NODDROPSHW: dprintf((" ** DDERR_NODDROPSHW\n")); break;
	case DDERR_NOMIRRORHW: dprintf((" ** DDERR_NOMIRRORHW\n")); break;
	case DDERR_NORASTEROPHW: dprintf((" ** DDERR_NORASTEROPHW\n")); break;
	case DDERR_NOROTATIONHW: dprintf((" ** DDERR_NOROTATIONHW\n")); break;
	case DDERR_NOSTRETCHHW: dprintf((" ** DDERR_NOSTRETCHHW\n")); break;
	case DDERR_NOZBUFFERHW: dprintf((" ** DDERR_NOZBUFFERHW\n")); break;
	case DDERR_SURFACEBUSY: dprintf((" ** DDERR_SURFACEBUSY\n")); break;
	case DDERR_SURFACELOST: dprintf((" ** DDERR_SURFACELOST\n")); break;
	case DDERR_UNSUPPORTED: dprintf((" ** DDERR_UNSUPPORTED\n")); break;
	case DDERR_WASSTILLDRAWING: dprintf((" ** DDERR_WASSTILLDRAWING\n")); break;
	default: dprintf((" ** debugBlt(): don't know\n")); break;
	}
}

#endif // DEBUG_ON