#include "groupsource.h"
#include <iostream>

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

ALfloat GroupSource::FilterDistance(ALuint source,Speaker speaker) {
  ALfloat gain,maxdist,refdist,rolloff,position[3],direction[3];
  ALfloat iangle,oangle,ogain;
  ALint relative;
  alGetSourcefv(source,AL_GAIN_LINEAR_LOKI,&gain);
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
  // TODO: Get left/right displacement.. Somehow..
  if(speaker==Left)
    ; // Displace listener to the left
  else
    ; // Displace it to the right

  if(relative==AL_FALSE) {  // Don't need to move it if it's relative..
    listener[0]-=position[0];
    listener[1]-=position[1];
    listener[2]-=position[2];
  }
  // Ok, listener moved, so that listener is actually a direction vector.
  // Now, we just have to normalize it, to calculate angle.
  // direction should (?) already be normalized.
  float dist=sqrt(listener[0]*listener[0]+
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
    cerr << "theta<(iangle/2.0)\n";
    // Should be empty, as it uses "normal" attenuation, as computed above
  } else if(theta<(oangle/2.0)) {
    cerr << "theta<(oangle/2.0)\n";
    gain+=0.01;     // This is a bit weird.. Shouldn't ogain be part of this?
    gain/=2.0;
  } else {
    cerr << "theta>(oangle/2.0)\n";
    if(ogain<0.01)
      gain=0.01;
    else
      gain=ogain;
  }
  cerr << "gain=" << gain << "\n";
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
void GroupSource::ApplyFilters(Source *source,ALshort *buffer,ALsizei size) {
  //TODO: Implement
  //1. Init (Monoify? - No!) (SplitSources? - Not really necessary..)
  //2. Apply filters: doppler,pitch,{da,reverb,coning,minmax},listenergain
  //                                 *   *      *      *         -
  //   * coning & da could probably be one filter.
  //   * listenergain should be removed, otherwise it will be applied twice 
  //3. "_alSourceParamApply"
  //TODO: This should be done for each speaker... Or, actually, it (probably)
  //      only has to be done for Distance (da+coning)..
  //      Another caveat: Because the data will have to be stored in a buffer,
  //      we have to use stereo (or mono) format. So there's no point in
  //      calculating anything for more than two channels.
  ALuint sourcename=source->GetAlSource();
  ALfloat pitch,lgain,rgain;
  alGetSourcefv(sourcename,AL_PITCH,&pitch);
  pitch*=FilterDoppler(sourcename);
  // Apply this pitch
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
}

//TODO: Add frequency-parameter???
void GroupSource::MixSources() throw (InitError,FileError,FatalError) {
  ALshort *loaddata=NULL,*data=NULL,*bdata=NULL;
  ALsizei bsize=0,size=0,loadsize=0,bits,freq;
  ALenum format;
  ALboolean success;
    
  if(sources_.size()<1)
    throw InitError("Sources must be included before trying to mix");
  
  cerr << "Load first WAV (" 
       << ((Sample &)sources_[0]->GetSound()).GetFileName() << ")\n";
  success=
    alutLoadWAV(((Sample &)sources_[0]->GetSound()).GetFileName().c_str(),
		(ALvoid **)&loaddata,&format,&loadsize,&bits,&freq);
  if(success==AL_FALSE)
    throw FileError("Error opening file for mixing");

  bdata=(ALshort *)_alBufferCanonizeData(format,loaddata,loadsize,freq,
					 AL_FORMAT_STEREO16,
					 22050,(ALuint *)&bsize,AL_FALSE);  
  if(!bdata)
    throw FatalError("Error converting data to internal format!");

  free(loaddata);

  cerr << "Apply filters to it\n";
  ApplyFilters(sources_[0],bdata,bsize);

  for(unsigned int s=1;s<sources_.size();s++) {
    unsigned int ss=s+1;
    cerr << "Load " << ss << ((ss==1)?"st":(ss==2)?"nd":(ss==3)?"rd":"th")
	 << " WAV\n";
    success=
      alutLoadWAV(((Sample &)sources_[s]->GetSound()).GetFileName().c_str(),
		  (ALvoid **)&loaddata,&format,&loadsize,&bits,&freq);
    if(success==AL_FALSE)
      throw FileError("Error opening file for mixing");
    
    cerr << "Convert it\n";
    data=(ALshort *)_alBufferCanonizeData(format,loaddata,loadsize,freq,
					  AL_FORMAT_STEREO16,
					  22050,(ALuint *)&size,AL_FALSE);  
    if(!data)
      throw FatalError("Error converting data to internal format!");

    free(loaddata);

    cerr << "Apply filters to it\n";
    ApplyFilters(sources_[s],data,size);

    if(size>bsize) {
      cerr << "size>bsize\n";
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
    cerr << "bdata+=data\n";
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
    
  cerr << "bsize=" << bsize << "\n";
    
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
