#ifndef WINDOWSSTUFF_H_INCLUDED
#define WINDOWSSTUFF_H_INCLUDED
#ifdef WIN32

#include <AL/alut.h>

#define alGetSourceiv alGetSourcei
inline ALboolean alutLoadWAV(const char *fname,ALvoid **buffer,ALint *format,
			     ALsizei *size,ALsizei *bits,ALsizei *freq) {
  alutLoadWAVFile(const_cast<char *>(fname),format,buffer,size,bits,
		  (ALbyte *)freq);
  return AL_TRUE;
}

#endif
#endif
