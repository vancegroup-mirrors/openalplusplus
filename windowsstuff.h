#ifndef WINDOWSSTUFF_H_INCLUDED
#define WINDOWSSTUFF_H_INCLUDED
#ifdef WIN32

#include <AL/alut.h>

#define alGetSourceiv alGetSourcei
inline ALboolean alutLoadWAV(const char *fname,ALvoid **buffer,ALint *format,
			     ALsizei *size,ALsizei *bits,ALsizei *freq) {
  ALboolean loop;
  alutLoadWAVFile(const_cast<char *>(fname),format,buffer,size,freq,&loop);
  if(*format==AL_FORMAT_STEREO16 || *format==AL_FORMAT_MONO16)
    *bits=16;
  else
    *bits=8;
  return AL_TRUE;
}

// This should take care of VC++ strange way of handling loop variables...
#define for if(0); else for

#endif
#endif
