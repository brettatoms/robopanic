//debugset.h
//Edit this file to change debug settings.
//There's no need to include this anyplace else -
//it's already included where it needs to be.

//Copyright 1999, Ian Parberry.  All rights reserved.

#ifndef __DEBUGSET_H__
#define __DEBUGSET_H__

//#define DEBUG_ON //define this for debugging

//output modes, select any or all

#ifdef DEBUG_ON
  //define for output to file
  #define DEBUG_OUTPUT_TO_FILE
  //define for output to debugger
  #define DEBUG_OUTPUT_TO_DEBUGGER
#endif

//settings for output modes

#ifdef DEBUG_OUTPUT_TO_FILE
  //file for debug output
  #define DEBUG_FILENAME "debug.txt"
#endif

#endif //__DEBUGSET_H__
