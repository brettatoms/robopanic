//debug.h
//Include this file everywhere that you want debug output
//except for your main - include debugdec.hpp instead.

#include "debugset.h" //debug settings

//declaration of debug manager class CDebugManager

#ifndef __DEBUG_H__ //to prevent compiler seeing this twice

#ifdef DEBUG_ON //debug code is not compiled unless needed

#include <windows.h>
#include <stdio.h>

#define DEBUG_OUTBUF_SIZE 1024 //size of output buffer
#define DEBUG_FNAME_SIZE 64 //size of file name

class CDebugManager{ //the debug manager
  private:
    FILE *m_fileHandle; //handle for debug output file
    char m_szOutBuffer[DEBUG_OUTBUF_SIZE]; //output buffer
    char m_szOutBuffer2[DEBUG_OUTBUF_SIZE]; //another buffer
    char m_szFileName[DEBUG_FNAME_SIZE]; //file message is from
    int m_nLineNumber; //line message is from
  public:
    CDebugManager(); //constructor
    ~CDebugManager(); //destructor
    void printf(const char* format,...); //debug printf
    void setsource(char* file,int line); //set file, line num
};

#endif //DEBUG_ON
#endif //__DEBUG_H__

//Copyright 1999, Ian Parberry.  All rights reserved.

//external declaration of debug manager

#ifdef DEBUG_ON
  //CDebugManager defined in debugman.hpp
  extern CDebugManager g_cDebugManager;
#endif //DEBUG_ON

//global function calls used for debugging
#ifndef __DEBUG_H__ //to prevent compiler seeing this twice

#ifdef DEBUG_ON
//debug printf
//thanks to Dale Addink for this idea.
//Remember to use double parens.
//Example: dprintf(("This is a test of %d\n",foo));
#define dprintf(A) \
{ \
  g_cDebugManager.setsource(__FILE__,__LINE__); \
  g_cDebugManager.printf A; \
}
#else
#define dprintf(A) //nothing
#endif //DEBUG_ON

//final defines
#define __DEBUG_H__ //this must be the last define 

#endif //__DEBUG_H__
