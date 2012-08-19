//sound.cpp: sound manager
//Copyright Ian Parberry, 1997, 1998
//Last updated Nov. 18, 199


#include <dsound.h>
#include "defines.h"
#include "sound.h"
#include <stdio.h>
#include "debug.h"

CSoundManager::CSoundManager(HWND hwnd){ //constructor
  m_Hwnd=hwnd; //save hwnd
  m_nCount=0; //no sounds yet
  //null out sound buffers and flags
  for(int i=0; i<MAX_SOUNDS; i++){
    m_lpBuffer[i]=NULL; m_nCopyCount[i]=0;
  }
  //start direct sound
  m_bOperational=DirectSoundCreate(NULL,&m_lpDirectSound,NULL)==DS_OK;
  if(!m_bOperational)return;
  m_bOperational=m_lpDirectSound->SetCooperativeLevel(hwnd,DSSCL_NORMAL)==DS_OK;
}

CSoundManager::~CSoundManager(){ //destructor
  if(!m_bOperational)return;
  clear(); //clear all buffers
  (void)m_lpDirectSound->Release(); //release direct sound
}

////////////////////////////////////////////////////////

void CSoundManager::clear(){ //clear all sounds
  if(!m_bOperational)return;
  stop(); //stop all sounds (paranoia)
  //for(int i=0; i<m_nCount; i++){ //for each sound
  for(int i=0; i<NUM_SOUNDS; i++){ //for each sound
    for(int j=0; j<m_nCopyCount[i]; j++){ //for each copy
      m_lpBuffer[i][j]->Release(); //release the sound
      m_lpBuffer[i][j]=NULL; //probably not needed
    }
    //reclaim memory
    delete[]m_lpBuffer[i];
  }
  m_nCount=0; //no sounds left (hopefully)
}

////////////////////////////////////////////////////////

LPDSoundBuffer CSoundManager::createBuffer(char *filename){
	BYTE *sound = NULL;
	int length = LoadSound(filename, &sound);
	if(length <= 0) return NULL;
	if(!m_bOperational) return NULL; //bail if not initialized correctly
	DSBUFFERDESC dsbdesc;
	PCMWAVEFORMAT pcmwf;	
	//init pcmwf, wave format descriptor
	memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
	pcmwf.wf.wFormatTag =		WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels =		DS_CHANSPERSAMPLE;
	pcmwf.wf.nSamplesPerSec =	DS_SAMPLERATE;
	pcmwf.wf.nBlockAlign =		DS_CHANSPERSAMPLE*DS_BITSPERSAMPLE/8;
	pcmwf.wf.nAvgBytesPerSec =	pcmwf.wf.nSamplesPerSec*pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample =		DS_BITSPERSAMPLE;
	//init dsbdesc, direct sound buffer descriptor
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbdesc.dwBufferBytes = length;
	dsbdesc.lpwfxFormat=(LPWAVEFORMATEX)&pcmwf;
	//create sound buffers and return success code
	//m_lpBuffer[index]=new LPDIRECTSOUNDBUFFER[copies];
	//LPDIRECTSOUNDBUFFER buffer = new LPDIRECTSOUNDBUFFER;
	LPDIRECTSOUNDBUFFER buffer = NULL;
	
	//for(int i=0; i<copies; i++) m_lpBuffer[index][i]=NULL;
	//return m_lpDirectSound->CreateSoundBuffer(&dsbdesc, &buffer, NULL) 
		//== DS_OK ? buffer : NULL;
	HRESULT r = m_lpDirectSound->CreateSoundBuffer(&dsbdesc, &buffer, NULL);
	LoadBuffer(buffer, sound, length);
	return buffer;

}

////////////////////////////////////////////////////////

void CSoundManager::load(char *filename, int type, int copies){ //load sound from file
//copies is how many copies of sound are to play simultaneously
  int length; //length of sound
  BYTE *sound=NULL; //temporary buffer to hold sound data

  //bail out if necessary
  if(!m_bOperational) return; //bail if not initialized correctly
  //if(m_nCount>=MAX_SOUNDS)return; //bail if no room for new sound

  //load sound data from file
  length = LoadSound(filename, &sound); //load sound from file
  /*
  CreateBuffer(m_nCount,length,copies); //create multiple directsound buffers
  LoadBuffer(m_nCount,sound,length); //load sound into buffer
  CopyBuffer(m_nCount,copies); //make copies of buffer
  */
  CreateBuffer(type ,length,copies); //create multiple directsound buffers
  LoadBuffer(type ,sound,length); //load sound into buffer
  CopyBuffer(type ,copies); //make copies of buffer
  

  //clean up and exit
  delete[]sound; //delete temporary sound buffer
  m_nCount++; //increment counter
  return;
}

////////////////////////////////////////////////////////

void CSoundManager::play(int index, BOOL looping){ //play sound
  if(!m_bOperational)return; //bail if not initialized correctly
  //if(index<0||index>=m_nCount)return; //bail if bad index
  if(index<0 || index>=NUM_SOUNDS)return; //bail if bad index
  int copy=0; //current copy
  DWORD status; //status of that copy

  //get status of first copy
  if(m_lpBuffer[index][copy]->GetStatus(&status) != DS_OK) //attempt to get status
    status=DSBSTATUS_PLAYING; //assume playing if cannot get status

  //find next unplayed copy, if any
  while(copy<m_nCopyCount[index] && (status & DSBSTATUS_PLAYING)){ //while in use
    if(++copy<m_nCopyCount[index]) //go to next copy
      if(m_lpBuffer[index][copy]->GetStatus(&status) != DS_OK) //attempt to get status
        status=DSBSTATUS_PLAYING; //assume playing if cannot get status
  }

  //play copy
  if(copy<m_nCopyCount[index]) //if unused copy found
    m_lpBuffer[index][copy]->Play(0,0,looping?DSBPLAY_LOOPING:0); //play it
}

/////////////////////////////////////////////////////////

//void CSoundManager::play(LPLPDSoundBuffer *buffer, BOOL looping){ //play sound
void CSoundManager::play(LPDSoundBuffer buffer, BOOL looping){ //play sound
	dprintf(("entered CSoundManager::play(buffer)\n"));
	if(!m_bOperational)  return; //bail if not initialized correctly
	//if(index<0||index>=m_nCount)return; //bail if bad index
	//if(index<0||index>=NUM_SOUNDS)return; //bail if bad index
	int copy=0; //current copy
	DWORD status; //status of that copy

	//get status of first copy
	//if(m_lpBuffer[index][copy]->GetStatus(&status)!=DS_OK) //attempt to get status
	//if(buffer->GetStatus(&status) != DS_OK) //attempt to get status
		//status=DSBSTATUS_PLAYING; //assume playing if cannot get status

	HRESULT r = buffer->GetStatus(&status);
	if(r != DS_OK) { dprintf(("r != DD_OK\n")); }
	else if(r == DS_OK){ dprintf(("HRESULT == DS_OK\n")); }
	else dprintf(("dont know\n"));

	switch(status){
	case DSBSTATUS_BUFFERLOST:
		dprintf(("DSBSTATUS_BUFFERLOST \n")); break;
	case DSBSTATUS_LOOPING:
		dprintf(("DSBSTATUS_LOOPING\n")); break;
	case DSBSTATUS_PLAYING :
		dprintf(("DSBSTATUS_PLAYING \n")); break;		
	default: dprintf(("default\n")); break;
	}

	  //find next unplayed copy, if any
	/*
	while(copy<m_nCopyCount[index]&&(status&DSBSTATUS_PLAYING)){ //while in use
	    if(++copy<m_nCopyCount[index]) //go to next copy
		  if(m_lpBuffer[index][copy]->GetStatus(&status)!=DS_OK) //attempt to get status
			status=DSBSTATUS_PLAYING; //assume playing if cannot get status
	}*/

	//play copy
	//if(copy<m_nCopyCount[index]) //if unused copy found

    r = buffer->Play(0, 0, looping?DSBPLAY_LOOPING:0); //play it
	if(r == DS_OK) { dprintf(("playing OK\n")); }
	else dprintf(("problem playing \n"));
	dprintf(("leaving CSoundManager::play(buffer)\n"));
}

/////////////////////////////////////////////////////////

void CSoundManager::stop(int index){ //stop playing sound
  if(!m_bOperational)return; //bail if not initialized correctly
  //if(index<0||index>=m_nCount)return; //bail if bad index
  if(index<0||index>=NUM_SOUNDS)return; //bail if bad index
  for(int i=0; i<m_nCopyCount[index]; i++){ //for each copy
    m_lpBuffer[index][i]->Stop(); //stop playing
    m_lpBuffer[index][i]->SetCurrentPosition(0); //reset to start
  }
}

////////////////////////////////////////////////////////

void CSoundManager::stop(LPDSoundBuffer buffer){ //stop playing sound
	if(!m_bOperational)return; //bail if not initialized correctly
	if(buffer != NULL) buffer->Stop();
}

////////////////////////////////////////////////////////

void CSoundManager::stop(void){ //stop playing sound
  if(!m_bOperational)return; //bail if not initialized correctly
  //for(int index=0; index<m_nCount; index++)
  for(int index=0; index<NUM_SOUNDS; index++)
	  stop(index); //stop them all
}

////////////////////////////////////////////////////////

BOOL CSoundManager::CopyBuffer(int index, int copies){ //make copies of buffer
  if(!m_bOperational)  return FALSE; //bail if not initialized correctly
  BOOL result=TRUE; //TRUE if everything went OK
  m_nCopyCount[index]=copies; //record number of copies of this sound
  for(int i=1; i<copies; i++)  //make copies
	  result = result && 
		m_lpDirectSound->DuplicateSoundBuffer(*m_lpBuffer[index],
		&(m_lpBuffer[index][i])) == DS_OK;
  return result;
}

////////////////////////////////////////////////////////

BOOL CSoundManager::CreateBuffer(int index, int length, int copies){
  if(!m_bOperational)return FALSE; //bail if not initialized correctly
  DSBUFFERDESC dsbdesc;
  PCMWAVEFORMAT pcmwf;
  if(length<=0)return FALSE; //bail if length info wrong
  //init pcmwf, wave format descriptor
  memset(&pcmwf,0,sizeof(PCMWAVEFORMAT));
  pcmwf.wf.wFormatTag=WAVE_FORMAT_PCM;
  pcmwf.wf.nChannels=DS_CHANSPERSAMPLE;
  pcmwf.wf.nSamplesPerSec=DS_SAMPLERATE;
  pcmwf.wf.nBlockAlign=DS_CHANSPERSAMPLE*DS_BITSPERSAMPLE/8;
  pcmwf.wf.nAvgBytesPerSec=pcmwf.wf.nSamplesPerSec*pcmwf.wf.nBlockAlign;
  pcmwf.wBitsPerSample=DS_BITSPERSAMPLE;
  //init dsbdesc, direct sound buffer descriptor
  memset(&dsbdesc,0,sizeof(DSBUFFERDESC));
  dsbdesc.dwSize=sizeof(DSBUFFERDESC);
  dsbdesc.dwFlags=DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
  dsbdesc.dwBufferBytes=length;
  dsbdesc.lpwfxFormat=(LPWAVEFORMATEX)&pcmwf;
  //create sound buffers and return success code
  m_lpBuffer[index]=new LPDIRECTSOUNDBUFFER[copies];
  for(int i=0; i<copies; i++)m_lpBuffer[index][i]=NULL;
  return m_lpDirectSound->
    CreateSoundBuffer(&dsbdesc,m_lpBuffer[index],NULL)==DS_OK;
}

////////////////////////////////////////////////////////

BOOL CSoundManager::LoadBuffer(LPDIRECTSOUNDBUFFER buffer, BYTE *sound, int length){
  if(!m_bOperational) return FALSE; //bail if not initialized correctly
  LPVOID w1, w2; //write pointer (use 2 for buffer wraparound)
  DWORD l1, l2; //length of sound to be written to write pointers
  //LPDIRECTSOUNDBUFFER buffer = *m_lpBuffer[index];
  if(length<=0)  return FALSE; //bail if length info wrong
  if(buffer->Lock(0, length, &w1, &l1, &w2, &l2, 0) //lock down buffer 
	  ==DSERR_BUFFERLOST){ //if buffer lost
    buffer->Restore(); //restore
    if(buffer->Lock(0, length, &w1, &l1, &w2, &l2,0) != DS_OK) //and try again
      return FALSE; //abort if failed the second time
  }
  CopyMemory(w1, sound, l1); //load the first half of the sound
  if(w2 != NULL)  CopyMemory(w2, sound+l1, l2); //load the second half
  //unlock the buffer and return
  if(buffer->Unlock(w1, l1, w2, l2) != DS_OK)  return FALSE;
  return TRUE;
}

////////////////////////////////////////////////////////

BOOL CSoundManager::LoadBuffer(int index, BYTE *sound, int length){
  if(!m_bOperational)return FALSE; //bail if not initialized correctly
  LPVOID w1,w2; //write pointer (use 2 for buffer wraparound)
  DWORD l1,l2; //length of sound to be written to write pointers
  LPDIRECTSOUNDBUFFER buffer = *m_lpBuffer[index];
  if(length<=0)  return FALSE; //bail if length info wrong
  if(buffer->Lock(0,length,&w1,&l1,&w2,&l2,0) //lock down buffer 
	  ==DSERR_BUFFERLOST){ //if buffer lost
    buffer->Restore(); //restore
    if(buffer->Lock(0,length,&w1,&l1,&w2,&l2,0)!=DS_OK) //and try again
      return FALSE; //abort if failed the second time
  }
  CopyMemory(w1,sound,l1); //load the first half of the sound
  if(w2!=NULL)  CopyMemory(w2,sound+l1,l2); //load the second half
  //unlock the buffer and return
  if(buffer->Unlock(w1,l1,w2,l2) != DS_OK)  return FALSE;
  return TRUE;
}

////////////////////////////////////////////////////////

int CSoundManager::LoadSound(char *filename, BYTE **buffer){
  if(!m_bOperational)  return 0; //bail if not initialized correctly
  HMMIO hmfr;
  MMCKINFO parent,child;
  WAVEFORMATEX wfmtx;
  int size=0;
  
  //reclaim memory from buffer, if already used
  delete[]*buffer;
  //init parent and child MMCKINFOs
  parent.ckid=(FOURCC)0;
  parent.cksize=0;
  parent.fccType=(FOURCC)0;
  parent.dwDataOffset=0;
  parent.dwFlags=0;
  child=parent;
  //open sound file
  hmfr=mmioOpen(filename,NULL,MMIO_READ|MMIO_ALLOCBUF);
  if(hmfr==NULL)  return NULL;
  //descend into the RIFF
  parent.fccType = mmioFOURCC('W','A','V','E');
  if(mmioDescend(hmfr,&parent,NULL,MMIO_FINDRIFF)){
    mmioClose(hmfr, 0); return NULL; //not a wave file
  }
  //descend to the WAVEfmt
  child.ckid = mmioFOURCC('f','m','t',' ');
  if(mmioDescend(hmfr,&child,&parent,0)){
    mmioClose(hmfr, 0); return NULL; //WAVE file has no fmt chunk
  }
  //read the WAVEFMT from the wave file
  if(mmioRead(hmfr, (char*)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx)){
    mmioClose(hmfr, 0); return NULL; //unable to read fmt chunk
  }
  //check wave format
  if(wfmtx.wFormatTag != WAVE_FORMAT_PCM){
    mmioClose(hmfr, 0); return NULL; //WAVE file is not PCM format
  }
  //ascend back to RIFF level
  if(mmioAscend(hmfr, &child, 0)){
    mmioClose(hmfr, 0); return NULL; //unable to ascend
  }
  //descend to the data chunk
  child.ckid = mmioFOURCC('d','a','t','a');
  if(mmioDescend(hmfr, &child, &parent, MMIO_FINDCHUNK)){
    mmioClose(hmfr, 0); return NULL; //WAVE file has no data chunk
  }
  //grab memory to store sound
  size = child.cksize;
  *buffer = new BYTE[size];
  if(*buffer == NULL){
    mmioClose(hmfr, 0); return NULL; //out of memory
  }
  //read the wave data
  //if((DWORD)mmioRead(hmfr,(char *)*buffer,size)!=size){
  if(mmioRead(hmfr, (char *)*buffer, size) != size){  
	  mmioClose(hmfr,0); delete[]*buffer; return NULL; //data read failed
  }
  //close file and return
  mmioClose(hmfr,0);
  return size;
}

