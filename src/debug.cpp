//debug.cpp
//CDebugManager functions

//Copyright 1999, Ian Parberry.  All rights reserved.

#include "debug.h"

#ifdef DEBUG_ON //debug code not compiled unless needed

CDebugManager::CDebugManager(){ //constructor
  #ifdef DEBUG_OUTPUT_TO_FILE
    //open output file
    m_fileHandle=fopen(DEBUG_FILENAME,"wt");
  #endif
}

CDebugManager::~CDebugManager(){ //destructor
  #ifdef DEBUG_OUTPUT_TO_FILE
    fclose(m_fileHandle); //close output file
  #endif
}

//set file name and line number
void CDebugManager::setsource(char* file,int line){
  char* p=strrchr(file,'\\');
  if(p==NULL)strcpy(m_szFileName,file); //set file name
  else strcpy(m_szFileName,(char*)(p+1));
  m_nLineNumber=line; //set line number
}

//debug printf
void CDebugManager::printf(const char* format,...){
  //print file and line number to m_szOutBuffer
  sprintf(m_szOutBuffer,
    "%s(%d): ",m_szFileName,m_nLineNumber);
  //append debug message from parameter list
  va_list arglist;
  va_start(arglist,format);
  vsprintf((char*)(m_szOutBuffer+strlen(m_szOutBuffer)),
    format,arglist);
  va_end(arglist);
  //print m_szOutBuffer to file if needed
  #ifdef DEBUG_OUTPUT_TO_FILE
  fprintf(m_fileHandle,"%s",m_szOutBuffer);
  fflush(m_fileHandle);
  #endif
  //print m_szOutBuffer to debugger if needed
  #ifdef DEBUG_OUTPUT_TO_DEBUGGER
  OutputDebugString(m_szOutBuffer);
  #endif
}

#endif
