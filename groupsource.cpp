#include "groupsource.h"
#include <iostream>
#include <stdlib.h>

// TODO: malloc/free should be changed to new/delete (as much as possible)

namespace openalpp {
  
GroupSource::GroupSource(float x,float y,float z) throw (NameError)
  : SourceBase(x,y,z),mixed_(false) {
  alGenBuffers(1,&buffer_);
  if(alGetError()!=AL_FALSE)
    throw NameError("Error generating buffer name.");
  alSourcei(sourcename_,AL_BUFFER,buffer_);
}

void GroupSource::Play() throw (InitError,FileError) {
  try {
    if(!mixed_)
      MixSources();
  } catch(InitError error) {
    throw InitError("Sources must be included before trying to play");
  }
  SourceBase::Play();
}

ALfloat FilterDoppler(ALuint source) {
  // TODO: Implement.. This isn't really necessary, for obvious reasons...
  return 1.0;
}

#ifndef WIN32
#define AL_GAIN_LINEAR_LOKI                      0x20000
#endif

ALfloat GroupSource::FilterDistance(ALuint source,Speaker speaker) {
  ALfloat gain,maxdist,refdist,rolloff,position[3],direction[3],orientation[6];
  ALfloat iangle,oangle,ogain,dist,right[3];
  ALint relative;
#ifdef WIN32
  alGetSourcefv(source,AL_GAIN,&gain);
#else
  alGetSourcefv(source,AL_GAIN_LINEAR_LOKI,&gain);
#endif
  alGetSourcefv(source,AL_MAX_DISTANCE,&maxdist);
  alGetSourcefv(source,AL_REFERENCE_DISTANCE,&refdist);
  alGetSourcefv(source,AL_ROLLOFF_FACTOR,&rolloff);
  alGetSourcefv(source,AL_POSITION,position);
  alGetSourcefv(source,AL_DIRECTION,direction);
  alGetSourcefv(source,AL_CONE_INNER_ANGLE,&iangle);
  alGetSourcefv(source,AL_CONE_OUTER_ANGLE,&oangle);
  alGetSourcefv(source,AL_CONE_OUTER_GAIN,&ogain);
  alGetSourceiv(source,AL_SOURCE_RELATIVE,&relative);
  
  float theta=0;
  ALfloat listener[3];
  alGetListenerfv(AL_POSITION,listener);
  alGetListenerfv(AL_ORIENTATION,orientation);
  //Normalize at and up vectors
  dist=sqrt(orientation[0]*orientation[0]+
	    orientation[1]*orientation[1]+
	    orientation[2]*orientation[2]);
  if(dist<0.0001)
    throw FatalError("Faulty at vector in AL_ORIENTATION!");
  orientation[0]/=dist;
  orientation[1]/=dist;
  orientation[2]/=dist;
  dist=sqrt(orientation[3]*orientation[3]+
	    orientation[4]*orientation[4]+
	    orientation[5]*orientation[5]);
  if(dist<0.0001)
    throw FatalError("Faulty up vector in AL_ORIENTATION!");
  orientation[3]/=dist;
  orientation[4]/=dist;
  orientation[5]/=dist;

  // TODO: The speaker position calculation should be moved so it's done
  // _once_ instead of once per source and speaker..

  // A vector pointing to the right from the listener can be found by
  // calculating the cross product of at and up.
  // rightx=aty*upz-upy*atz
  // righty=upx*atz-atx*upz
  // rightz=atx*upy-upx*aty
  // TODO: Will this always be normalized?
  right[0]=orientation[1]*orientation[5]-orientation[4]*orientation[2];
  right[1]=orientation[3]*orientation[2]-orientation[0]*orientation[5];
  right[2]=orientation[0]*orientation[4]-orientation[3]*orientation[1];
  
  if(speaker==Left) {
    listener[0]-=5.0*right[0];
    listener[1]-=5.0*right[1];
    listener[2]-=5.0*right[2];
  } else {
    listener[0]+=5.0*right[0];
    listener[1]+=5.0*right[1];
    listener[2]+=5.0*right[2];
  }

  if(relative==AL_FALSE) {  // Don't need to move it if it's relative..
    listener[0]-=position[0];
    listener[1]-=position[1];
    listener[2]-=position[2];
  }
  // Ok, listener moved, so that listener is actually a direction vector.
  // Now, we just have to normalize it, to calculate angle.
  // direction must also be normalized.
  dist=sqrt(direction[0]*direction[0]+
	    direction[1]*direction[1]+
	    direction[2]*direction[2]);
  if(dist>0.0001) {
    direction[0]/=dist;
    direction[1]/=dist;
    direction[2]/=dist;
  }
  dist=sqrt(listener[0]*listener[0]+
	    listener[1]*listener[1]+
	    listener[2]*listener[2]);
  if(dist>0.0001) {
    listener[0]/=dist;
    listener[1]/=dist;
    listener[2]/=dist;
    float dot=listener[0]*direction[0]+listener[1]*direction[1]+
      listener[2]*direction[2];
    theta=acos(dot);
  }

  int almodel;
  alGetIntegerv(AL_DISTANCE_MODEL,&almodel);
  switch(almodel) {
    case(AL_NONE):
      break;
    case(AL_INVERSE_DISTANCE_CLAMPED):
      if(dist<refdist)
	dist=refdist;
      if(dist>maxdist)
	dist=maxdist;
    case(AL_INVERSE_DISTANCE):
      gain*=refdist/(refdist+rolloff*(dist-refdist));
      if(gain>1.0)
	gain=1.0;
      if(gain<0.0)
	gain=0.0;
  }  

  if(theta<(iangle/2.0)) {
    std::cerr << "theta<(iangle/2.0)\n";
    // Should be empty, as it uses "normal" attenuation, as computed above
  } else if(theta<(oangle/2.0)) {
    std::cerr << "theta<(oangle/2.0)\n";
    gain+=0.01;     // This is a bit weird.. Shouldn't ogain be part of this?
    gain/=2.0;
  } else {
    std::cerr << "theta>(oangle/2.0)\n";
    if(ogain<0.01)
      gain=0.01;
    else
      gain=ogain;
  }
  std::cerr << "gain=" << gain << "\n";
  return gain;
}

void GroupSource::FilterReverb(Source *source,ALshort *buffer,ALsizei size) {
  if(!reverbinitiated_)
    return;
  // out=in[i]+scale*in[i-delay]
  ALfloat delay,scale;
  delay=source->GetReverbDelay();
  scale=source->GetReverbScale();
  if(delay==0.0 || scale==0.0)
    return;

  // Modify delay; 2 is for 2 channels (stereo)
  //               22050*2 is the frequency*2 (= #samples)
  int idelay=(int)(delay*2.0*22050.0*2.0);

  // TODO: realloc buffer to be idelay bigger.. 

  ALint amp;
  for(unsigned int i=idelay;i<(size/sizeof(ALshort));i++) {
    amp=(ALint)buffer[i]+(ALint)(scale*buffer[i-idelay]);
    if(amp>32767)
      amp=32767;
    else if(amp<-32768)
      amp=-32768;
    buffer[i]=(ALshort)amp;
  }
}

/**
 * Apply filters to source.
 */
ALshort *GroupSource::ApplyFilters(Source *source,ALshort *buffer,
				   ALsizei &size){
  //TODO: Implement
  //  Apply filters: doppler,pitch,{da,reverb,coning,minmax},listenergain
  //                           +     *   *      *      *         -
  //   * coning & da could probably be one filter => Done!
  //   * remove listenergain, otherwise it will be applied twice => Done!

  ALuint sourcename=source->GetAlSource();
  ALfloat pitch,lgain,rgain;

  //Pitch
  alGetSourcefv(sourcename,AL_PITCH,&pitch);
  pitch*=FilterDoppler(sourcename);
  if(pitch>0.0) {
    ALsizei nsize=(ALsizei)(size/pitch);
    ALshort *nbuffer=(ALshort *)malloc(nsize*sizeof(ALshort));
    ALfloat index=0.0;
    for(int i=0;i<(nsize/2);i++) {
      if(index*2>size)
	throw FatalError("index>size - we're all doomed!");
      int iindex=(int)index;
      float fracindex=index-(float)iindex;
      int destl=i*2,destr=i*2+1;
      int sourcel=iindex*2,sourcer=iindex*2+1;

      // Left channel
      nbuffer[destl]=buffer[sourcel];
      if((sourcel+2)<size)
	nbuffer[destl]+=(ALshort)fracindex*(buffer[sourcel+2]-
					    buffer[sourcel]);

      // Right channel
      nbuffer[destr]=buffer[sourcer];
      if((sourcer+2)<size)
	nbuffer[destr]+=(ALshort)fracindex*(buffer[sourcer+2]-
					    buffer[sourcer]);
      index+=pitch;
    }
    free(buffer);
    buffer=nbuffer;
    size=nsize;
  }

  lgain=FilterDistance(sourcename,Left);
  rgain=FilterDistance(sourcename,Right);
  FilterReverb(source,buffer,size);
  ALfloat min,max;                          // minmax filter
  alGetSourcefv(sourcename,AL_MIN_GAIN,&min);
  alGetSourcefv(sourcename,AL_MAX_GAIN,&max);

  if(lgain<min)
    lgain=min;
  else if(lgain>max)
    lgain=max;
  if(rgain<min)
    rgain=min;
  else if(rgain>max)
    rgain=max;

  for(unsigned int i=0;i<(size/sizeof(ALshort));i++) {
    if(i%2)
      buffer[i]=(ALshort)(buffer[i]*rgain);
    else
      buffer[i]=(ALshort)(buffer[i]*lgain);
  }
  return buffer;
}

/*
 * TODO: The next 1300 lines, or so, are copied from AL source.. I did this 
 * so I wouldn't be depending on people having the linux source, but I didn't
 * realise it would be so much... I'll fix it up a bit, i.e. make it C++ code,
 * and probably move it to a file of its own. I'll also try to figure out if I
 * really need all of this, as it's pretty general code.
 */

typedef struct _acAudioCVT {
  int needed;                     /* Set to 1 if conversion possible */
  ALushort src_format;            /* Source audio format */
  ALushort dst_format;            /* Target audio format */
  double rate_incr;               /* Rate conversion increment */
  void   *buf;                    /* Buffer to hold entire audio data */
  int    len;                     /* Length of original audio buffer */
  int    len_cvt;                 /* Length of converted audio buffer */
  int    len_mult;                /* buffer must be len*len_mult big */
  double len_ratio;       /* Given len, final size is len*len_ratio */
  void (*filters[10])(struct _acAudioCVT *cvt, ALushort format);
  int filter_index;               /* Current audio conversion function */
} acAudioCVT;

/* Audio format flags (defaults to LSB byte order) */
#define AUDIO_U8        0x0008  /* Unsigned 8-bit samples */
#define AUDIO_S8        0x8008  /* Signed 8-bit samples */
#define AUDIO_U16LSB    0x0010  /* Unsigned 16-bit samples */
#define AUDIO_S16LSB    0x8010  /* Signed 16-bit samples */
#define AUDIO_U16MSB    0x1010  /* As above, but big-endian byte order */
#define AUDIO_S16MSB    0x9010  /* As above, but big-endian byte order */

/* Native audio byte ordering */
#ifndef WORDS_BIGENDIAN
#define AUDIO_U16       AUDIO_U16LSB
#define AUDIO_S16       AUDIO_S16LSB
#define swap16le(x) (x)
#define swap32le(x) (x)
#define swap16be(x) swap16(x)
#define swap32be(x) swap32(x)
#else
#define AUDIO_U16       AUDIO_U16MSB
#define AUDIO_S16       AUDIO_S16MSB
#define swap16le(x) swap16(x)
#define swap32le(x) swap32(x)
#define swap16be(x) (x)
#define swap32be(x) (x)
#endif

#define _al_ALCHANNELS(fmt) ((fmt==AL_FORMAT_MONO16||fmt==AL_FORMAT_MONO8)?1:2)

#define DATA            0x61746164              /* "data" */
#define FACT            0x74636166              /* "fact" */
#define LIST            0x5453494c              /* "LIST" */
#define RIFF            0x46464952
#define WAVE            0x45564157
#define FMT             0x20746D66

#define AL_FORMAT_IMA_ADPCM_MONO16_EXT            0x10000
#define AL_FORMAT_IMA_ADPCM_STEREO16_EXT          0x10001
#define AL_FORMAT_WAVE_EXT                        0x10002

#define NELEMS(x) ((sizeof x) / (sizeof *x))

#define PCM_CODE        0x0001
#define MS_ADPCM_CODE   0x0002
#define IMA_ADPCM_CODE  0x0011

#define MS_ADPCM_max ((1<<(16-1))-1)
#define MS_ADPCM_min -(1<<(16-1))

ALboolean _al_RAWFORMAT(ALenum format) {
  std::cerr << "_al_RAWFORMAT\n";
  switch(format) {
    case AL_FORMAT_MONO16:
    case AL_FORMAT_MONO8:
    case AL_FORMAT_STEREO16:
    case AL_FORMAT_STEREO8:
      return AL_TRUE;
    default:
      break;
  }
  return AL_FALSE;
}

/* Convert rate up by multiple of 2 */
void acFreqMUL2(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acFreqMUL2\n";

  int i;
  ALubyte *src, *dst;

  src = (ALubyte *) cvt->buf + cvt->len_cvt;
  dst = (ALubyte *) cvt->buf + cvt->len_cvt * 2;

  switch(format & 0xFF) {
    case 8:
      for ( i=cvt->len_cvt; i; --i ) {
	src -= 1;
	dst -= 2;
	dst[0] = src[0];
	dst[1] = src[0];
      }
      break;
    case 16:
      for ( i=cvt->len_cvt/2; i; --i ) {
	src -= 2;
	dst -= 4;
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[0];
	dst[3] = src[1];
      }
      break;
  }
  cvt->len_cvt *= 2;
}

/* Convert rate down by multiple of 2 */
void acFreqDIV2(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acFreqDIV2\n";

  int i;
  ALubyte *src, *dst;

  src = (ALubyte *)cvt->buf;
  dst = (ALubyte *)cvt->buf;
  switch(format & 0xFF) {
    case 8:
      for ( i=cvt->len_cvt/2; i; --i ) {
	dst[0] = src[0];
	src += 2;
	dst += 1;
      }
      break;
    case 16:
      for ( i=cvt->len_cvt/4; i; --i ) {
	dst[0] = src[0];
	dst[1] = src[1];
	src += 4;
	dst += 2;
      }
      break;
  }

  cvt->len_cvt /= 2;
}

/* Very slow rate conversion routine */
void acFreqSLOW(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acFreqSLOW\n";

  double ipos;
  int i, clen;

  clen = (int) ((double)cvt->len_cvt / cvt->rate_incr);
  if(cvt->rate_incr > 1.0) {
    switch(format & 0xFF) {
      case 8: {
	ALubyte *output;
	
	output = (ALubyte *)cvt->buf;
	ipos = 0.0;
	for ( i=clen; i; --i ) {
	  *output = *((ALubyte *) cvt->buf + (int)ipos);
	  ipos += cvt->rate_incr;
	  output += 1;
	}
      } break;
      case 16: {
	ALushort *output;

	clen &= ~1;
	output = (ALushort *) cvt->buf;
	ipos = 0.0;
	for ( i=clen/2; i; --i ) {
	  *output=((ALushort *)cvt->buf)[(int)ipos];
	  ipos += cvt->rate_incr;
	  output += 1;
	}
      } break;
    }
  } else {
    switch (format & 0xFF) {
      case 8: {
	ALubyte *output;
	
	output = (ALubyte *) cvt->buf + clen;
	ipos = (double)cvt->len_cvt;
	for ( i=clen; i; --i ) {
	  ipos -= cvt->rate_incr;
	  output -= 1;
	  *output = *((ALubyte *) cvt->buf + (int)ipos);
	}
      } break;
    case 16: {
      ALushort *output;
      
      clen &= ~1;
      output = (ALushort *) cvt->buf;
      output += clen / sizeof *output;

      ipos = (double)cvt->len_cvt/2;
      for ( i=clen/2; i; --i ) {
	ipos -= cvt->rate_incr;
	output -= 1;
	*output=((ALushort *)cvt->buf)[(int)ipos];
      }
    } break;
    }
  }
  cvt->len_cvt = clen;
}


/* Duplicate a mono channel to both stereo channels */
void acConvertStereo(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acConvertStereo\n";

  int i;

  if((format & 0xFF) == 16) {
    ALushort *src, *dst;

    src  = (ALushort *) cvt->buf;
    dst  = (ALushort *) cvt->buf;
    src += cvt->len_cvt / sizeof *src;
    dst += cvt->len_cvt * 2 / sizeof *dst;

    for ( i=cvt->len_cvt/2; i; --i ) {
      dst -= 2;
      src -= 1;
      dst[0] = src[0];
      dst[1] = src[0];
    }
  } else {
    ALubyte *src, *dst;
		
    src = (ALubyte *) cvt->buf + cvt->len_cvt;
    dst = (ALubyte *) cvt->buf + cvt->len_cvt * 2;
    for(i = cvt->len_cvt; i; i-- ) {
      dst -= 2;
      src -= 1;
      dst[0] = src[0];
      dst[1] = src[0];
    }
  }

  cvt->len_cvt *= 2;
}

/* Effectively mix right and left channels into a single channel */
void acConvertMono(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acConvertMono\n";

  int i;
  ALint sample;

  switch(format & 0x8018) {
    case AUDIO_U8: {
      ALubyte *src, *dst;

      src = (ALubyte *)cvt->buf;
      dst = (ALubyte *)cvt->buf;
      for ( i=cvt->len_cvt/2; i; --i ) {
	sample = src[0] + src[1];
	sample /= 2;
	
	if ( sample > 255 )
	  *dst = 255;
	else
	  *dst = sample;
	src += 2;
	dst += 1;
      }
    } break;
    case AUDIO_S8: {
      ALbyte *src, *dst;
      
      src = (ALbyte *)cvt->buf;
      dst = (ALbyte *)cvt->buf;
      for ( i=cvt->len_cvt/2; i; --i ) {
	sample = src[0] + src[1];
	sample /= 2;

	if ( sample > 127 ) {
	  *dst = 127;
	} else
	  if ( sample < -128 )
	    *dst = -128;
	  else
	    *dst = sample;
	src += 2;
	dst += 1;
      }
    } break;
    case AUDIO_U16: {
      ALubyte *src, *dst;
      
      src = (ALubyte *)cvt->buf;
      dst = (ALubyte *)cvt->buf;
      if ( (format & 0x1000) == 0x1000 ) {
	for ( i=cvt->len_cvt/4; i; --i ) {
	  sample = (ALushort)((src[0]<<8)|src[1])+
	    (ALushort)((src[2]<<8)|src[3]);
	  sample /= 2;
	  
	  if ( sample > 65535 ) {
	    dst[0] = 0xFF;
	    dst[1] = 0xFF;
	  } else {
	    dst[1] = (sample&0xFF);
	    sample >>= 8;
	    dst[0] = (sample&0xFF);
	  }
	  src += 4;
	  dst += 2;
	}
      } else {
	for ( i=cvt->len_cvt/4; i; --i ) {
	  sample = (ALushort)((src[1]<<8)|src[0])+
	    (ALushort)((src[3]<<8)|src[2]);
	  sample /= 2;

	  if ( sample > 65535 ) {
	    dst[0] = 0xFF;
	    dst[1] = 0xFF;
	  } else {
	    dst[0] = (sample&0xFF);
	    sample >>= 8;
	    dst[1] = (sample&0xFF);
	  }
	  src += 4;
	  dst += 2;
	}
      }
    } break;
    case AUDIO_S16: {
      ALubyte *src, *dst;
      
      src = (ALubyte *)cvt->buf;
      dst = (ALubyte *)cvt->buf;
      if ( (format & 0x1000) == 0x1000 ) {
	for ( i=cvt->len_cvt/4; i; --i ) {
	  sample = (ALshort)((src[0]<<8)|src[1])+
	    (ALshort)((src[2]<<8)|src[3]);
	  sample /= 2;

	  if ( sample > 32767 ) {
	    dst[0] = 0x7F;
	    dst[1] = 0xFF;
	  } else
	    if ( sample < -32768 ) {
	      dst[0] = 0x80;
	      dst[1] = 0x00;
	    } else {
	      dst[1] = (sample&0xFF);
	      sample >>= 8;
	      dst[0] = (sample&0xFF);
	    }
	  src += 4;
	  dst += 2;
	}
      } else {
	for ( i=cvt->len_cvt/4; i; --i ) {
	  sample = (ALshort)((src[1]<<8)|src[0])+
	    (ALshort)((src[3]<<8)|src[2]);
	  sample /= 2;
	  
	  if ( sample > 32767 ) {
	    dst[1] = 0x7F;
	    dst[0] = 0xFF;
	  } else
	    if ( sample < -32768 ) {
	      dst[1] = 0x80;
	      dst[0] = 0x00;
	    } else {
	      dst[0] = (sample&0xFF);
	      sample >>= 8;
	      dst[1] = (sample&0xFF);
	    }
	  src += 4;
	  dst += 2;
	}
      }
    }
    break;
  }
  cvt->len_cvt /= 2;
}

/* Toggle signed/unsigned */
void acConvertSign(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acConvertSign\n";

  int i;
  ALubyte *data;

  data = (ALubyte *)cvt->buf;
  if((format & 0xFF) == 16) {
    if((format & 0x1000) != 0x1000) /* Little endian */
      ++data;

    for(i = cvt->len_cvt/2; i; i-- ) {
      *data ^= 0x80;
      data += 2;
    }
  } else
    for(i = cvt->len_cvt; i; i-- )
      *data++ ^= 0x80;

  format = (format ^ 0x8000);

  return;
}

/* Toggle endianness */
void acConvertEndian(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acConvertEndian\n";

  int i;
  ALubyte *data, tmp;

  data = (ALubyte *)cvt->buf;
  for(i = cvt->len_cvt/2; i; i--) {
    tmp     = data[0];
    data[0] = data[1];
    data[1] = tmp;
    data   += 2;
  }

  format = (format ^ 0x1000);
}

/* Convert 8-bit to 16-bit - LSB */
void acConvert16LSB( acAudioCVT *cvt, ALushort format ) {
  std::cerr << "acConvert16LSB\n";

  int i;
  ALubyte *src, *dst;

  src = (ALubyte *) cvt->buf + cvt->len_cvt;
  dst = (ALubyte *) cvt->buf + cvt->len_cvt * 2;
  for(i = cvt->len_cvt; i; i-- ) {
    src -= 1;
    dst -= 2;
    dst[0] = 0;
    dst[1] = *src;
  }

  format = ((format & ~0x0008) | AUDIO_U16LSB);
  cvt->len_cvt *= 2;

  return;
}

/* Convert 8-bit to 16-bit - MSB */
void acConvert16MSB(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acConvertMSB\n";

  int i;
  ALubyte *src, *dst;
  
  src = (ALubyte *) cvt->buf + cvt->len_cvt;
  dst = (ALubyte *) cvt->buf + cvt->len_cvt * 2;
  for(i = cvt->len_cvt; i; i--) {
    src -= 1;
    dst -= 2;
    dst[1] = 0;
    dst[0] = *src;
  }

  format = ((format & ~0x0008) | AUDIO_U16MSB);
  cvt->len_cvt *= 2;
}


/* Convert 16-bit to 8-bit */
void acConvert8(acAudioCVT *cvt, ALushort format) {
  std::cerr << "acConvert8\n";

  int i;
  ALubyte *src, *dst;

  src = (ALubyte *)cvt->buf;
  dst = (ALubyte *)cvt->buf;

  if((format & 0x1000) != 0x1000) /* Little endian */
    ++src;

  for(i = cvt->len_cvt/2; i; i--) {
    *dst = *src;
    src += 2;
    dst += 1;
  }
  format = ((format & ~0x9010) | AUDIO_U8);

  cvt->len_cvt /= 2;
}

int acBuildAudioCVT(acAudioCVT *cvt,
		    ALushort src_format, ALubyte src_channels, ALuint src_rate,
		    ALushort dst_format, ALubyte dst_channels, ALuint dst_rate) {

  std::cerr << "acBuildAudioCVT\n";

  /* Start off with no conversion necessary */
  cvt->needed       = 0;
  cvt->filter_index = 0;
  cvt->filters[0]   = NULL;
  cvt->len_mult     = 1;
  cvt->len_ratio    = 1.0;

  /* First filter:  Endian conversion from src to dst */
  if((( src_format & 0x1000) != (dst_format & 0x1000)) &&
     (( src_format & 0xff) != 8))
    cvt->filters[cvt->filter_index++] = acConvertEndian;
        
  /* Third filter: Sign conversion -- signed/unsigned */
  if((src_format & 0x8000) != (dst_format & 0x8000))
    cvt->filters[cvt->filter_index++] = acConvertSign;

  /* Second filter:  Convert 16 bit <--> 8 bit PCM */
  if((src_format & 0xFF) != (dst_format & 0xFF))
    switch(dst_format & 0x10FF) {
      case AUDIO_U8:
	cvt->filters[cvt->filter_index++] =
	  acConvert8;
	cvt->len_ratio /= 2;
	break;
      case AUDIO_U16LSB:
	cvt->filters[cvt->filter_index++] =
	  acConvert16LSB;
	cvt->len_mult *= 2;
	cvt->len_ratio *= 2;
	break;
      case AUDIO_U16MSB:
	cvt->filters[cvt->filter_index++] =
	  acConvert16MSB;
	cvt->len_mult *= 2;
	cvt->len_ratio *= 2;
	break;
      default:
	break;
    }

  /* Last filter:  Mono/Stereo conversion */
  if(src_channels != dst_channels) {
    while((src_channels * 2) <= dst_channels) {
      cvt->filters[cvt->filter_index++] = 
	acConvertStereo;
      src_channels   *= 2;
      cvt->len_mult  *= 2;
      cvt->len_ratio *= 2;
    }

    /*
     * This assumes that 4 channel audio is in the format:
     * Left {front/back} + Right {front/back}
     * so converting to L/R stereo works properly.
     */
    while(((src_channels%2) == 0) &&
	  ((src_channels/2) >= dst_channels)) {
      cvt->filters[cvt->filter_index++] =
	acConvertMono;
      src_channels   /= 2;
      cvt->len_ratio /= 2;
    }
    if ( src_channels != dst_channels ) {
      /* Uh oh.. */;
    }
  }

  /* Do rate conversion */
  cvt->rate_incr = 0.0;
  if((src_rate / 100) != (dst_rate / 100)) {
    ALuint hi_rate, lo_rate;
    int len_mult;
    double len_ratio;
    void (*rate_cvt)(acAudioCVT *, ALushort );

    if ( src_rate > dst_rate ) {
      hi_rate   = src_rate;
      lo_rate   = dst_rate;
      rate_cvt  = acFreqDIV2;
      len_mult  = 1;
      len_ratio = 0.5;
    } else {
      hi_rate   = dst_rate;
      lo_rate   = src_rate;
      rate_cvt  = acFreqMUL2;
      len_mult  = 2;
      len_ratio = 2.0;
    }
    /* If hi_rate = lo_rate*2^x then conversion is easy */
    while(((lo_rate * 2)/100) <= (hi_rate/100)) {
      cvt->filters[cvt->filter_index++] = rate_cvt;
      lo_rate        *= 2;
      cvt->len_mult  *= len_mult;
      cvt->len_ratio *= len_ratio;
    }
    
    /* We may need a slow conversion here to finish up */
    if((lo_rate/100) != (hi_rate/100)) {
      /* The problem with this is that if the input buffer is
	 say 1K, and the conversion rate is say 1.1, then the
	 output buffer is 1.1K, which may not be an acceptable
	 buffer size for the audio driver (not a power of 2)
      */
      /* For now, punt and hope the rate distortion isn't great.
       */
      if ( src_rate < dst_rate ) {
	cvt->rate_incr = (double)lo_rate/hi_rate;
	cvt->len_mult *= 2;
	cvt->len_ratio /= cvt->rate_incr;
      } else {
	cvt->rate_incr = (double)hi_rate/lo_rate;
	cvt->len_ratio *= cvt->rate_incr;
      }
      cvt->filters[cvt->filter_index++] = acFreqSLOW;
    }
  }

  /* Set up the filter information */
  if(cvt->filter_index != 0) {
    cvt->needed     = 1;
    cvt->len        = 0;
    cvt->buf        = NULL;
    cvt->src_format = src_format;
    cvt->dst_format = dst_format;
    cvt->filters[cvt->filter_index] = NULL;
  }

  return cvt->needed;
}

/*
 * _al_AC2ALFMT( ALuint acformat, ALuint channels )
 *
 * Returns the openal format equivilant to the audioconvert format acformat,
 * with the number of channels specified by channels.
 */
ALenum _al_AC2ALFMT( ALuint acformat, ALuint channels ) {
  std::cerr << "_al_AC2ALFMT\n";

  switch( acformat ) {
    case AUDIO_U8:
      if(channels == 2)
	return AL_FORMAT_STEREO8;
      if(channels == 1)
	return AL_FORMAT_MONO8;
      break;
    case AUDIO_S16LSB:
    case AUDIO_S16MSB:
      if(channels == 2)
	return AL_FORMAT_STEREO16;
      if(channels == 1)
	return AL_FORMAT_MONO16;
      break;
  }
  return -1;
}

/*
 * _al_AL2ACFMT( ALenum alfmt )
 *
 * Returns the equivilant (sort of) audioconvert format specified by alfmt.
 * audioconvert formats do not have channel information, so this should be
 * combined with _al_ALCHANNELS.
 */
ALshort _al_AL2ACFMT( ALenum alformat ) {
  std::cerr << "_al_AL2ACFMT\n";

  switch( alformat ) {
    case AL_FORMAT_STEREO8:
    case AL_FORMAT_MONO8:
      return AUDIO_U8;
    case AL_FORMAT_STEREO16:
    case AL_FORMAT_MONO16:
      return AUDIO_S16;
    default:
      break;
  }
  return -1;
}

int acConvertAudio(acAudioCVT *cvt) {
  std::cerr << "acConvertAudio\n";

  int i;

  /* Make sure there's data to convert */
  if(cvt->buf == NULL)
    return -1;

  /* Return okay if no conversion is necessary */
  cvt->len_cvt = cvt->len;
  if(cvt->filters[0] == NULL) {
    return 0;
  }

  /* Set up the conversion and go! */
  cvt->filter_index = 0;
  for(i = 0; cvt->filters[i]; i++) {
    cvt->filters[i](cvt, cvt->src_format);
  }

  return 0;
}

struct MS_ADPCM_decodestate_FULL {
  ALubyte hPredictor;
  ALushort iDelta;
  ALshort iSamp1;
  ALshort iSamp2;
};

typedef struct WaveFMT {
        ALushort encoding;
        ALushort channels;               /* 1 = mono, 2 = stereo */
        ALuint frequency;              /* One of 11025, 22050, or 44100 Hz */
        ALuint byterate;               /* Average bytes per second */
        ALushort blockalign;             /* Bytes per sample block */
        ALushort bitspersample;
} alWaveFMT_LOKI;

typedef struct IMA_ADPCM_decodestate_s {
        ALint valprev;  /* Previous output value */
        ALbyte index;           /* Index into stepsize table */
} alIMAADPCM_decodestate_LOKI;

typedef struct IMA_ADPCM_decoder {
        alWaveFMT_LOKI wavefmt;
        ALushort wSamplesPerBlock;
        /* * * */
        alIMAADPCM_decodestate_LOKI state[2];
} alIMAADPCM_state_LOKI;

static struct MS_ADPCM_decoder_FULL {
  alWaveFMT_LOKI wavefmt;
  ALushort wSamplesPerBlock;
  ALushort wNumCoef;
  ALshort aCoeff[7][2];
  /* * * */
  struct MS_ADPCM_decodestate_FULL state[2];
} MS_ADPCM_state_FULL;

ALushort swap16(ALushort D) {
  return (D<<8) | (D>>8);
}

ALuint swap32(ALuint D) {
  return (D<<24) | ((D<<8)&0x00FF0000) | ((D>>8)&0x0000FF00) | (D>>24);
}

static ALint IMA_ADPCM_nibble(alIMAADPCM_decodestate_LOKI *state,
			      ALubyte nybble) {
  const ALint max_audioval = ((1<<(16-1))-1);
  const ALint min_audioval = -(1<<(16-1));
  const int index_table[16] = {
    -1, -1, -1, -1,
    2,  4,  6,  8,
    -1, -1, -1, -1,
    2,  4,  6,  8
  };
  const ALint step_table[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31,
    34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130,
    143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408,
    449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282,
    1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327,
    3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630,
    9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350,
    22385, 24623, 27086, 29794, 32767
  };
  ALint delta, step;

  /* Compute difference and new sample value */
  step = step_table[(int) state->index];
  delta = step >> 3;
  if ( nybble & 0x04 ) delta += step;
  if ( nybble & 0x02 ) delta += (step >> 1);
  if ( nybble & 0x01 ) delta += (step >> 2);
  if ( nybble & 0x08 ) delta = -delta;
  state->valprev += delta;
  
  /* Update index value */
  state->index += index_table[nybble];
  if ( state->index > 88 ) {
    state->index = 88;
  } else
    if ( state->index < 0 ) {
      state->index = 0;
    }
  
  /* Clamp output sample */
  if ( state->valprev > max_audioval ) {
    state->valprev = max_audioval;
  } else
    if ( state->valprev < min_audioval ) {
      state->valprev = min_audioval;
    }
  return(state->valprev);
}

/* Fill the decode buffer with a channel block of data (8 samples) */
static void Fill_IMA_ADPCM_block(ALubyte *decoded, ALubyte *encoded,
				 int channel, int numchannels,
				 alIMAADPCM_decodestate_LOKI *state){
  int i;
  ALbyte  nybble;
  ALint new_sample;

  decoded += (channel * 2);
  for ( i=0; i<4; ++i ) {
    nybble = (*encoded)&0x0F;
    new_sample = IMA_ADPCM_nibble(state, nybble);
    decoded[0] = new_sample&0xFF;
    new_sample >>= 8;
    decoded[1] = new_sample&0xFF;
    decoded += 2 * numchannels;
    
    nybble = (*encoded)>>4;
    new_sample = IMA_ADPCM_nibble(state, nybble);
    decoded[0] = new_sample&0xFF;
    new_sample >>= 8;
    decoded[1] = new_sample&0xFF;
    decoded += 2 * numchannels;
    
    ++encoded;
  }
}

int IMA_ADPCM_decode(ALubyte *indata, ALubyte *outdata,
		     ALuint len, alIMAADPCM_state_LOKI *istate, int offset) {
  alIMAADPCM_decodestate_LOKI *state;
  int indata_len;
  int c, channels = istate->wavefmt.channels;
  int samplesleft;

  indata += offset;
  state = istate->state;

  if(len < istate->wavefmt.blockalign)
    return -1;

  indata_len = len;
  /* Get ready... Go! */
  while ( indata_len >= istate->wavefmt.blockalign ) {
    /* Grab the initial information for this block */
    for ( c=0; c<channels; ++c ) {
      /* Fill the state information for this block */
      state[c].valprev = ((indata[1]<<8)|indata[0]);
      indata += 2;
      if ( state[c].valprev & 0x8000 )
	state[c].valprev -= 0x10000;
      state[c].index = *indata++;
      /* Reserved byte in buffer header, should be 0 */
      if ( *indata++ != 0 )
	/* Uh oh, corrupt data?  Buggy code? */;
      
      /* Store the initial valprev we start with */
      outdata[0] = state[c].valprev&0xFF;
      outdata[1] = state[c].valprev>>8;
      outdata += 2;
    }

    /* Decode and store the other samples in this block */
    samplesleft = (istate->wSamplesPerBlock-1)*channels;
    while ( samplesleft > 0 ) {
      for ( c=0; c<channels; ++c ) {
	Fill_IMA_ADPCM_block(outdata, indata,
			     c, channels, &state[c]);
	indata += 4;
	samplesleft -= 8;
      }
      outdata += (channels * 8 * 2);
    }
    indata_len -= istate->wavefmt.blockalign;
  }
  
  return 0;
}


int IMA_ADPCM_decode_FULL(alIMAADPCM_state_LOKI *istate,
			  ALubyte **audio_buf, ALuint *audio_len) {
  std::cerr << "IMA_ADPCM_decode_FULL\n";
  ALubyte *freeable, *encoded;
  ALint encoded_len;
  unsigned int channels;
	

        
  /* Check to make sure we have enough variables in the state array */
  channels = istate->wavefmt.channels;
  if ( channels > NELEMS(istate->state) )
    return(-1);

  /* Allocate the proper sized output buffer */
  encoded_len = *audio_len;
  encoded = *audio_buf;
  freeable = *audio_buf;
  *audio_len = (encoded_len/istate->wavefmt.blockalign) * 
    istate->wSamplesPerBlock*
    istate->wavefmt.channels*sizeof(ALshort);

  *audio_buf = (ALubyte *)malloc(*audio_len);
  if ( *audio_buf == NULL )
    return(-1);

  return IMA_ADPCM_decode(encoded, *audio_buf,
			  encoded_len, istate, 0);
}

static ALint MS_ADPCM_nibble_FULL(struct MS_ADPCM_decodestate_FULL *state,
				  ALubyte nybble, ALshort *coeff) {
  const ALint adaptive[] = {
    230, 230, 230, 230, 307, 409, 512, 614,
    768, 614, 512, 409, 307, 230, 230, 230
  };
  ALint new_sample, delta;

  new_sample = ((state->iSamp1 * coeff[0]) +
		(state->iSamp2 * coeff[1]))/256;

  if(nybble & 0x08) {
    new_sample += state->iDelta * (nybble-0x10);
  } else {
    new_sample += state->iDelta * nybble;
  }
  
  if(new_sample < MS_ADPCM_min) {
    new_sample = MS_ADPCM_min;
  } else if(new_sample > MS_ADPCM_max) {
    new_sample = MS_ADPCM_max;
  }

  delta = ((ALint) state->iDelta * adaptive[nybble]);
  if(delta < 4096) {
    delta = 16;
  } else {
    delta /= 256;
  }
  
  state->iDelta = delta;
  state->iSamp2 = state->iSamp1;
  state->iSamp1 = new_sample;

  return new_sample;
}

static int MS_ADPCM_decode_FULL(ALubyte **audio_buf, ALuint *audio_len) {
  std::cerr << "MS_ADPCM_decode_FULL\n";
  struct MS_ADPCM_decodestate_FULL *state[2];
  ALubyte *freeable, *encoded, *decoded;
  ALint encoded_len, samplesleft;
  ALbyte nybble, stereo;
  ALshort *coeff[2];
  ALint new_sample;

  /* Allocate the proper sized output buffer */
  encoded_len = *audio_len;
  encoded = *audio_buf;
  freeable = *audio_buf;
  *audio_len = (encoded_len/MS_ADPCM_state_FULL.wavefmt.blockalign) * 
    MS_ADPCM_state_FULL.wSamplesPerBlock*
    MS_ADPCM_state_FULL.wavefmt.channels*sizeof(ALshort);
  *audio_buf = (ALubyte *)malloc(*audio_len);
  if(*audio_buf == NULL)
    return -1;
  decoded = *audio_buf;

  /* Get ready... Go! */
  stereo = (MS_ADPCM_state_FULL.wavefmt.channels == 2);
  state[0] = &MS_ADPCM_state_FULL.state[0];
  state[1] = &MS_ADPCM_state_FULL.state[(int) stereo];
  while ( encoded_len >= MS_ADPCM_state_FULL.wavefmt.blockalign ) {
    /* Grab the initial information for this block */
    state[0]->hPredictor = *encoded++;
    if ( stereo )
      state[1]->hPredictor = *encoded++;
    state[0]->iDelta = ((encoded[1]<<8)|encoded[0]);
    encoded += sizeof(ALshort);
    if ( stereo ) {
      state[1]->iDelta = ((encoded[1]<<8)|encoded[0]);
      encoded += sizeof(ALshort);
    }
    state[0]->iSamp1 = ((encoded[1]<<8)|encoded[0]);
    encoded += sizeof(ALshort);
    if ( stereo ) {
      state[1]->iSamp1 = ((encoded[1]<<8)|encoded[0]);
      encoded += sizeof(ALshort);
    }
    state[0]->iSamp2 = ((encoded[1]<<8)|encoded[0]);
    encoded += sizeof(ALshort);
    if ( stereo ) {
      state[1]->iSamp2 = ((encoded[1]<<8)|encoded[0]);
      encoded += sizeof(ALshort);
    }
    coeff[0] = MS_ADPCM_state_FULL.aCoeff[state[0]->hPredictor];
    coeff[1] = MS_ADPCM_state_FULL.aCoeff[state[1]->hPredictor];
    
    /* Store the two initial samples we start with */
    decoded[0] = state[0]->iSamp2&0xFF;
    decoded[1] = state[0]->iSamp2>>8;
    decoded += 2;
    if ( stereo ) {
      decoded[0] = state[1]->iSamp2&0xFF;
      decoded[1] = state[1]->iSamp2>>8;
      decoded += 2;
    }
    decoded[0] = state[0]->iSamp1&0xFF;
    decoded[1] = state[0]->iSamp1>>8;
    decoded += 2;
    if ( stereo ) {
      decoded[0] = state[1]->iSamp1&0xFF;
      decoded[1] = state[1]->iSamp1>>8;
      decoded += 2;
    }
    
    /* Decode and store the other samples in this block */
    samplesleft = (MS_ADPCM_state_FULL.wSamplesPerBlock-2)*
      MS_ADPCM_state_FULL.wavefmt.channels;
    while ( samplesleft > 0 ) {
      nybble = (*encoded)>>4;
      new_sample = MS_ADPCM_nibble_FULL(state[0],nybble,coeff[0]);
      decoded[0] = new_sample&0xFF;
      new_sample >>= 8;
      decoded[1] = new_sample&0xFF;
      decoded += 2;
      
      nybble = (*encoded)&0x0F;
      new_sample = MS_ADPCM_nibble_FULL(state[1],nybble,coeff[1]);
      decoded[0] = new_sample&0xFF;
      new_sample >>= 8;
      decoded[1] = new_sample&0xFF;
      decoded += 2;
      
      ++encoded;
      samplesleft -= 2;
    }
    encoded_len -= MS_ADPCM_state_FULL.wavefmt.blockalign;
  }
  
  /* free(freeable); */
  return 0;
}

int InitIMA_ADPCM(alIMAADPCM_state_LOKI *state, alWaveFMT_LOKI *format) {
  ALubyte  *rogue_feel;
  ALushort extra_info;

  /* Set the rogue pointer to the IMA_ADPCM specific data */
  state->wavefmt.encoding     = swap16le(format->encoding);
  state->wavefmt.channels     = swap16le(format->channels);
  state->wavefmt.frequency    = swap32le(format->frequency);
  state->wavefmt.byterate     = swap32le(format->byterate);
  state->wavefmt.blockalign   = swap16le(format->blockalign);
  state->wavefmt.bitspersample =
    swap16le(format->bitspersample);
  rogue_feel = (ALubyte *)format + sizeof(*format);
  if ( sizeof(*format) == 16 ) {
    extra_info = ((rogue_feel[1]<<8)|rogue_feel[0]);
    rogue_feel += sizeof(ALushort);
  }
  state->wSamplesPerBlock = ((rogue_feel[1]<<8)|rogue_feel[0]);
  
  state->state[0].valprev = 0;
  state->state[0].index   = 0;
  
  state->state[1].valprev = 0;
  state->state[1].index   = 0;
  
  return(0);
}

static int InitMS_ADPCM(alWaveFMT_LOKI *format) {
  ALubyte *rogue_feel;
  ALushort extra_info;
  int i;

  /* Set the rogue pointer to the MS_ADPCM specific data */
  MS_ADPCM_state_FULL.wavefmt.encoding = swap16le(format->encoding);
  MS_ADPCM_state_FULL.wavefmt.channels = swap16le(format->channels);
  MS_ADPCM_state_FULL.wavefmt.frequency = swap32le(format->frequency);
  MS_ADPCM_state_FULL.wavefmt.byterate = swap32le(format->byterate);
  MS_ADPCM_state_FULL.wavefmt.blockalign = swap16le(format->blockalign);
  MS_ADPCM_state_FULL.wavefmt.bitspersample =
                                         swap16le(format->bitspersample);
  rogue_feel = (ALubyte *)format+sizeof(*format);
  if(sizeof(*format) == 16) {
    extra_info = ((rogue_feel[1]<<8)|rogue_feel[0]);
    rogue_feel += sizeof(ALushort);
  }

  MS_ADPCM_state_FULL.wSamplesPerBlock = ((rogue_feel[1]<<8)|rogue_feel[0]);
  rogue_feel += sizeof(ALushort);
  MS_ADPCM_state_FULL.wNumCoef = ((rogue_feel[1]<<8)|rogue_feel[0]);
  rogue_feel += sizeof(ALushort);
  if ( MS_ADPCM_state_FULL.wNumCoef != 7 )
    return -1;

  for(i = 0; i < MS_ADPCM_state_FULL.wNumCoef; i++) {
    MS_ADPCM_state_FULL.aCoeff[i][0] =((rogue_feel[1]<<8)|rogue_feel[0]);
    rogue_feel += sizeof(ALushort);
    MS_ADPCM_state_FULL.aCoeff[i][1] =((rogue_feel[1]<<8)|rogue_feel[0]);
    rogue_feel += sizeof(ALushort);
  }

  return 0;
}

typedef struct Chunk {
  ALuint magic;
  ALuint length;
  void *data;
} Chunk;

/* read riff chunk */
static int ReadChunk(void *srcp, int offset, Chunk *chunk) {
  ALuint reader;
  ALubyte *src = ((ALubyte *)srcp)+offset;

  memcpy((void *) &reader, (void *) src, 4);
  chunk->magic    = swap32le(reader);

  memcpy((void *) &reader, (void *) (src+4), 4);
  chunk->length   = swap32le(reader);

  chunk->data     = (void *) (src+8);

  return chunk->length;
}

/*
 * convert wave to pcm data, setting size, fmt, chan, and freq.
 *
 * usually just a memcpy, but for MS_ADPCM does that too.
 */
void *ac_wave_to_pcm(void *data, ALuint *size,
		     ALushort *fmt, ALushort *chan, ALushort *freq) {
  void *retval;
  alWaveFMT_LOKI *format;
  Chunk riffchunk = { 0, 0, 0 };
  int offset = 12;
  long length;
  alIMAADPCM_state_LOKI ima_decoder;

  do {
    offset += (length=ReadChunk(data, offset, &riffchunk)) + 8;
    if(length < 0)
      return NULL;  
  } while ((riffchunk.magic == WAVE) ||
	   (riffchunk.magic == RIFF));                   
  if(riffchunk.magic != FMT)
    return NULL;

  format = (alWaveFMT_LOKI *) riffchunk.data;

  /* channels */
  *chan = swap16le(format->channels);

  /* freq */
  *freq = (ALushort) swap32le(format->frequency);
  switch(swap16le(format->encoding)) {
    case PCM_CODE:
      switch(swap16le(format->bitspersample)) {
        case 8:
	  *fmt = AUDIO_U8;
	  break;
        case 16:
	  *fmt = AUDIO_S16LSB;
	  break;
        default:
	  std::cerr << "Unknown bits " << swap16le(format->bitspersample) 
		    << "\n";
	  break;
      }
      do {
	length = ReadChunk(data, offset, &riffchunk);
	offset += (length + 8);
	if(length < 0)
	  return NULL;	
      } while(riffchunk.magic != DATA);

      retval = malloc(length);
      if(retval == NULL)
	return NULL;
      
      /* FIXME: Need to convert to native format? */
      memcpy(retval, riffchunk.data, length);
      *size = length;
      return retval;
      break;
    case MS_ADPCM_CODE:
      /* not possible to do an inplace conversion */
      *fmt = AUDIO_S16LSB;

      if(InitMS_ADPCM(format) < 0)
	return NULL;

      do {
	length = ReadChunk(data, offset, &riffchunk);
	offset += length + 8;
	
	retval = riffchunk.data;
      } while (riffchunk.magic != DATA);

      if(MS_ADPCM_decode_FULL((ALubyte **) &retval,
			      (ALuint *) &length) < 0)
                          return NULL;
      *size = length;
      return retval;
      break;
    case IMA_ADPCM_CODE:
      /* not possible to do an inplace conversion */
      *fmt = AUDIO_S16LSB;
      
      if(InitIMA_ADPCM(&ima_decoder, format) < 0)
	return NULL;
      
      do {
	length = ReadChunk(data, offset, &riffchunk);
	offset += length + 8;

	retval = riffchunk.data;
      } while (riffchunk.magic != DATA);
      if(IMA_ADPCM_decode_FULL(&ima_decoder,
			       (ALubyte **) &retval,
			       (ALuint *) &length) < 0)
	return NULL;

      *size = length;
      return retval;
      break;
    default:
      break;
  }

  return NULL;
}

void *acLoadWAV(void *data, ALuint *size, void **udata, 
                ALushort *fmt, ALushort *chan, ALushort *freq) {
  acAudioCVT endianConverter;
  
  if((data == NULL) || (udata == NULL) || (size == NULL) ||
     (fmt  == NULL) || (chan  == NULL) || (freq == NULL))
    return NULL;

  *udata = ac_wave_to_pcm(data, size, fmt, chan, freq);
  if(*udata == NULL)
    return NULL;


  if((*fmt == AUDIO_S8) || (*fmt == AUDIO_U8) || (*fmt == AUDIO_S16))
    return *udata;

  if(acBuildAudioCVT(&endianConverter,
		     /* from */
		     *fmt,
		     *chan,
		     *freq,

		     /* to */
		     AUDIO_S16,
		     *chan,
		     *freq) < 0) {
    throw FatalError("Couldn't build audio conversion data structure!");
    free(udata);
    return NULL;
  }

  endianConverter.buf = *udata;
  endianConverter.len = *size;

  acConvertAudio(&endianConverter);

  return endianConverter.buf;
}

static void *_alConvert( ALvoid *data,
			 ALenum f_format, ALuint f_size, ALuint f_freq,
			 ALenum t_format, ALuint t_freq, ALuint *retsize) {
  ALvoid *compressed = NULL;
  ALvoid *retval = NULL;
  acAudioCVT s16le;

  if((f_format == t_format) && (f_freq == t_freq)) {
    std::cerr << "No conversion needed.\n";
    /*
     * no conversion needed.
     */
    *retsize = f_size;

    retval = malloc( f_size );
    if( retval == NULL )
      throw MemoryError("Out of memory");

    memcpy( retval, data, f_size );
	
    return retval;
  }

  /*
   * Compressed auto formats like IMA_ADPCM get converted in
   * full here.
   */
  if(_al_RAWFORMAT(f_format) == AL_FALSE) {
    std::cerr << "Audio compressed\n";
    ALushort acfmt;
    ALushort achan;
    ALushort acfreq;

    switch(f_format) {
      case AL_FORMAT_IMA_ADPCM_MONO16_EXT:
      case AL_FORMAT_IMA_ADPCM_STEREO16_EXT:
      case AL_FORMAT_WAVE_EXT:
	acLoadWAV(data, &f_size, &retval, &acfmt, &achan, &acfreq);
	
	f_format = _al_AC2ALFMT(acfmt, achan);
	f_freq   = acfreq;
	break;
      default:
	break;
    }

    compressed = data = retval;
  }

  if(acBuildAudioCVT(&s16le,
		     /* from */
		     _al_AL2ACFMT(f_format),
		     _al_ALCHANNELS(f_format),
		     f_freq,

		     /* to */
		     _al_AL2ACFMT(t_format),
		     _al_ALCHANNELS(t_format),
		     t_freq) < 0) {
    throw FatalError("Couldn't build audio conversion data structure.");
    free(compressed);
    return NULL;
  }

  s16le.buf = retval = malloc(f_size * s16le.len_mult);
  if(retval == NULL) {
    free( compressed );
    throw MemoryError("Out of memory");
  }
  memcpy(retval, data, f_size);

  s16le.len = f_size;

  if(acConvertAudio(&s16le) < 0) {
    free( compressed );
    return NULL;
  }

  *retsize = s16le.len_cvt;

  if( s16le.buf != compressed )
    free( compressed );

  return s16le.buf;
}

//TODO: Add frequency-parameter???
void GroupSource::MixSources() throw (InitError,FileError,FatalError) {
  ALshort *loaddata=NULL,*data=NULL,*bdata=NULL;
  ALsizei bsize=0,size=0,loadsize=0,bits,freq;
  ALenum format;
  ALboolean success;
    
  if(sources_.size()<1)
    throw InitError("Sources must be included before trying to mix");
  
  std::cerr << "Load first WAV (" 
	    << ((Sample &)sources_[0]->GetSound()).GetFileName() << ")\n";
  success=
    alutLoadWAV(((Sample &)sources_[0]->GetSound()).GetFileName().c_str(),
		(ALvoid **)&loaddata,&format,&loadsize,&bits,&freq);
  if(success==AL_FALSE)
    throw FileError("Error opening file for mixing");

  bdata=(ALshort *)_alConvert(loaddata,format,loadsize,freq,
			      AL_FORMAT_STEREO16,
			      22050,(ALuint *)&bsize);

  if(!bdata)
    throw FatalError("Error converting data to internal format!");

  free(loaddata);

  std::cerr << "Apply filters to it\n";
  bdata=ApplyFilters(sources_[0],bdata,bsize);

  for(unsigned int s=1;s<sources_.size();s++) {
    unsigned int ss=s+1;
    std::cerr << "Load " << ss << ((ss==1)?"st":(ss==2)?"nd":(ss==3)?"rd":"th")
	      << " WAV\n";
    success=
      alutLoadWAV(((Sample &)sources_[s]->GetSound()).GetFileName().c_str(),
		  (ALvoid **)&loaddata,&format,&loadsize,&bits,&freq);
    if(success==AL_FALSE)
      throw FileError("Error opening file for mixing");
    
    std::cerr << "Convert it\n";
    data=(ALshort *)_alConvert(loaddata,format,loadsize,freq,
			       AL_FORMAT_STEREO16,
			       22050,(ALuint *)&size);  
    if(!data)
      throw FatalError("Error converting data to internal format!");

    free(loaddata);

    std::cerr << "Apply filters to it\n";
    data=ApplyFilters(sources_[s],data,size);

    if(size>bsize) {
      std::cerr << "size>bsize\n";
      loaddata=bdata;
      bdata=data;
      data=loaddata;
      loadsize=bsize;
      bsize=size;
      size=loadsize;
      /* FIXME (?): Now, why doesn't this work?
	 loaddata=(ALshort *)realloc(bdata,size);
	 if(!loaddata) {
	 free(bdata);
	 free(data);
	 throw MemoryError("Not enough memory for buffer");
	 }
	 bdata=loaddata;
	 cerr << "Clear bdata[bsize - (size-1)]\n";
	 for(int i=bsize;i<size;i++)
	 bdata[i]=0;
	 bsize=size;
      */
    } 
    std::cerr << "bdata+=data\n";
    ALint amp;
    for(int i=0;i<(size/2);i++) {
      amp=bdata[i]+data[i];
      if(amp>32767)
	amp=32767;
      else if(amp<-32768)
	amp=-32768;
      bdata[i]=(ALshort)amp;
    }
    
    free(data);
  }
    
  std::cerr << "bsize=" << bsize << "\n";
    
  alBufferData(buffer_,AL_FORMAT_STEREO16,bdata,bsize,22050);
  mixed_=true;
  
  free(bdata);
}
  
ALuint GroupSource::IncludeSource(Source *source) throw (ValueError) {
  if(source->IsStreaming())
    throw ValueError("Can't include streaming sources in group.");
  sources_.push_back(source);
  return source->GetAlSource();
}

void GroupSource::ExcludeSource(const Source &source) {
  ALuint sourcename=source.GetAlSource();
  ExcludeSource(sourcename);
}

void GroupSource::ExcludeSource(ALuint sourcename) {
  for(unsigned int i=0;i<sources_.size();i++) {
    if(sourcename==sources_[i]->GetAlSource()) {
      sources_[i]=sources_[sources_.size()-1];
      sources_.pop_back();
      return;
    }
  }
  throw NameError("Trying to exclude source that has not been included.");
}

GroupSource::GroupSource(const GroupSource &groupsource) {
  sources_=groupsource.sources_;
  mixed_=false;
  alGenBuffers(1,&buffer_);
  if(alGetError()!=AL_FALSE)
    throw NameError("Error generating buffer name.");
  alSourcei(sourcename_,AL_BUFFER,buffer_);
}

GroupSource::~GroupSource() {
  alDeleteBuffers(1,&buffer_);
}

GroupSource &GroupSource::operator=(const GroupSource &groupsource) {
  if(this!=&groupsource) {
    sources_.clear();
    sources_=groupsource.sources_;
    mixed_=false;
  }
  return *this;
}

}
