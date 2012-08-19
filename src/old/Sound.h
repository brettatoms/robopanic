//sound.hpp: header class for sound.cpp
//CSoundManager allows you to play multiple copies of each sound simultaneously

//Copyright Ian Parberry, 1997, 1998
//Last updated Nov. 4, 1999
// Used by Daniel Martinez

#ifndef __SOUND__
#define __SOUND__

//#define WIN32 //this seems to be necessary for correct compile, but generates warnings

#include <dsound.h> //direct sound
#include "sndlist.h" //list of sound names
#include "defines.h"

//#define MAX_SOUNDS 64 //maximum number of different sounds
#define MAX_SOUNDS NUM_SOUNDS//maximum number of different sounds

#define DS_NUMCHANNELS 8 //number of channels
#define DS_CHANSPERSAMPLE 1 //mono sound
#define DS_BITSPERSAMPLE 16 //8 bit sound
#define DS_SAMPLERATE 44100 //44KHz sampling


class CSoundManager{ //sound manager
  protected:
    int m_nCount; //number of sounds loaded
    
	BOOL m_bOperational; //has been initialized correctly	
    LPLPDIRECTSOUNDBUFFER m_lpBuffer[MAX_SOUNDS]; //direct draw sound buffers
    int m_nCopyCount[MAX_SOUNDS]; //how many copies of each sound can be played
	
    virtual BOOL CreateBuffer(int index,int length,int copies); //create sound buffer
  private:
    HWND m_Hwnd; //app's window handle
    LPDIRECTSOUND m_lpDirectSound; //direct sound object
    BOOL LoadBuffer(int index,BYTE *buffer,int length); //load sound buffer
    int LoadSound(char *filename, BYTE **sound); //load sound from file
	BOOL LoadBuffer(LPDIRECTSOUNDBUFFER buffer, BYTE *sound, int length);
    BOOL CopyBuffer(int index,int copies); //make copies of buffer
  public:
    CSoundManager(HWND hwnd); //constructor
    ~CSoundManager(); //destructor
    virtual void clear(); //clear all sounds
    
	// these 3 function let you create, play and stop a wav file not
	// contained/defined within the sound manager
	LPDSoundBuffer createBuffer(char *filename); //create sound buffer
	void play(LPDSoundBuffer buffer, BOOL looping); //play buffer
	void stop(LPDSoundBuffer buffer); //stop all sounds

	void load(char *filename, int type, int copies=1); //load sound from file
    void play(int index, BOOL looping=FALSE); //play sound
	
    void stop(int index); //stop playing sound
    void stop(void); //stop all sounds
};

#endif
