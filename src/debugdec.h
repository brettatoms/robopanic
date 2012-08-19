//debugdec.h - declaration of instance of debugger class
//Include this in your main file and nowhere else.

//Copyright 1999, Ian Parberry.  All rights reserved.

#ifndef __DEBUGDEC_H__
#define __DEBUGDEC_H__

#include "debug.h" //definition of CDebugManager

#ifdef DEBUG_ON
  CDebugManager g_cDebugManager; //the debug manager
#endif //DEBUG_ON

#endif //__DEBUGDEC_H__
