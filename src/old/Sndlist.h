//sndlist.h: list of sound definitions
//Last updated Nov. 4, 1999

#ifndef __SNDLIST__
#define __SNDLIST__

#define LOOP_SOUND TRUE

enum GameSoundType{ //sounds used in game engine
  I_SOUND, // sound in game
  R_SOUND,  //press I play a sound
  J_SOUND,  // same
  K_SOUND,  // same
  M_SOUND   // same


};

enum IntroSoundType{ //sounds used during the intro
  TITLE_SOUND=0, //sound used during title screen
  LOGO_SOUND, //signature chord
};

#endif
