#include "sourcebase.h"

/**
 * TODO: Should error checking be optional?
 * (a static bool throwflag in AudioBase)?
 */

namespace openalpp {

void SourceBase::Init() throw (MemoryError,NameError) {
  alGenSources(1,&sourcename_);
  if(alGetError()!=AL_FALSE)
    throw NameError("Couldn't generate source name");
  nlinkedsources_=1;
  alloclinkedsources_=2;
  linkedsources_=(ALuint *)malloc(sizeof(ALuint)*alloclinkedsources_);
  if(!linkedsources_)
    throw MemoryError("malloc failed");
  linkedsources_[0]=sourcename_;
  reverbscale_=0.25;
  reverbdelay_=0.0;
}

SourceBase::SourceBase() throw (MemoryError,NameError) : PositionedObject() {
  Init();
}

SourceBase::SourceBase(float x,float y,float z)
  throw (MemoryError,NameError) : PositionedObject() {
  
  Init();
  SetPosition(x,y,z);
}

SourceBase::~SourceBase() {
  alDeleteSources(1,&sourcename_);
  free(linkedsources_);
}
  
SourceBase::SourceBase(const SourceBase &sourcebase) {
  float a,b,c;
  sourcebase.GetPosition(a,b,c);
  SetPosition(a,b,c);
  sourcebase.GetVelocity(a,b,c);
  SetVelocity(a,b,c);
  SetPitch(sourcebase.GetPitch());
  sourcebase.GetDirection(a,b,c);
  SetDirection(a,b,c);
  sourcebase.GetSoundCone(a,b,c);
  SetSoundCone(a,b,c);
  sourcebase.GetMinMaxGain(a,b);
  SetMinMaxGain(a,b);
  SetReferenceDistance(sourcebase.GetReferenceDistance());
  SetRolloffFactor(sourcebase.GetRolloffFactor());
  SetMaxDistance(sourcebase.GetMaxDistance());
  SetLooping(sourcebase.IsLooping());
  SetGain(sourcebase.GetGain());
}

SourceBase &SourceBase::operator=(const SourceBase &sourcebase) {
  if(this!=&sourcebase) {
    float a,b,c;
    sourcebase.GetPosition(a,b,c);
    SetPosition(a,b,c);
    sourcebase.GetVelocity(a,b,c);
    SetVelocity(a,b,c);
    SetPitch(sourcebase.GetPitch());
    sourcebase.GetDirection(a,b,c);
    SetDirection(a,b,c);
    sourcebase.GetSoundCone(a,b,c);
    SetSoundCone(a,b,c);
    sourcebase.GetMinMaxGain(a,b);
    SetMinMaxGain(a,b);
    SetReferenceDistance(sourcebase.GetReferenceDistance());
    SetRolloffFactor(sourcebase.GetRolloffFactor());
    SetMaxDistance(sourcebase.GetMaxDistance());
    SetLooping(sourcebase.IsLooping());
    SetGain(sourcebase.GetGain());
  }
  return *this;
}

//##ModelId=3BD86A43017B
void SourceBase::Play() {
  alSourcePlayv(nlinkedsources_,linkedsources_);
}

//##ModelId=3BD86A5E01AC
void SourceBase::Pause() {
  alSourcePausev(nlinkedsources_,linkedsources_);
}

//##ModelId=3BD86A6E001E
void SourceBase::Stop() {
  alSourceStopv(nlinkedsources_,linkedsources_);
}

//##ModelId=3BD86A75032B
void SourceBase::Rewind() {
  alSourceRewindv(nlinkedsources_,linkedsources_);
}

//##ModelId=3BDD292C009B
SourceState SourceBase::GetState() const {
  ALint state;
  alGetSourceiv(sourcename_,AL_SOURCE_STATE,&state);
  switch(state) {
    case(AL_INITIAL):
      return Initial;
    case(AL_PLAYING):
      return Playing;
    case(AL_PAUSED):
      return Paused;
    case(AL_STOPPED):
      return Stopped;
    default:
      throw FatalError("AL_SOURCE_STATE is unknown");
  }
}

//##ModelId=3BD86A9603B5
void SourceBase::SetLooping(bool loop) {
  if(loop)
    alSourcei(sourcename_,AL_LOOPING,AL_TRUE);
  else
    alSourcei(sourcename_,AL_LOOPING,AL_FALSE);
}

//##ModelId=3BD86C3F0149
bool SourceBase::IsLooping() const {
  ALint looping;
  alGetSourceiv(sourcename_,AL_LOOPING,&looping);
  return (looping==AL_TRUE);
}

//##ModelId=3BD86B310322
void SourceBase::SetDirection(float directionx, float directiony, float directionz) {
  alSource3f(sourcename_,AL_DIRECTION,directionx,directiony,directionz);
}

//##ModelId=3BD86BEC01CC
void SourceBase::GetDirection(float &directionx, float &directiony, float &directionz) const {
  ALfloat direction[3];
  alGetSourcefv(sourcename_,AL_DIRECTION,direction);
  directionx=direction[0];
  directiony=direction[1];
  directionz=direction[2];
}

//##ModelId=3BD86F7100D7
void SourceBase::MakeOmniDirectional() {
  alSource3f(sourcename_,AL_DIRECTION,0.0,0.0,0.0);
}

//##ModelId=3BD86C6F00EE
void SourceBase::SetSoundCone(float innerangle, float outerangle, float outergain) {
  alSourcef(sourcename_,AL_CONE_INNER_ANGLE,innerangle);
  alSourcef(sourcename_,AL_CONE_OUTER_ANGLE,outerangle);
  alSourcef(sourcename_,AL_CONE_OUTER_GAIN,outergain);
}

//##ModelId=3BD86E0000CD
void SourceBase::GetSoundCone(float &innerangle, float &outerangle, float &outergain) const {
  alGetSourcefv(sourcename_,AL_CONE_INNER_ANGLE,&innerangle);
  alGetSourcefv(sourcename_,AL_CONE_OUTER_ANGLE,&outerangle);
  alGetSourcefv(sourcename_,AL_CONE_OUTER_GAIN,&outergain);
}

//##ModelId=3BD86E5002E4
void SourceBase::SetGain(float gain) {
  alSourcef(sourcename_,AL_GAIN,gain);
  ALenum error=alGetError();
  if(error!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError("Invalid value for gain");
	break;
      default:
	throw FatalError("Error trying to set gain!");
    }
}

//##ModelId=3BD86E5002EE
float SourceBase::GetGain() const {
  ALfloat gain;
  alGetSourcefv(sourcename_,AL_GAIN,&gain);
  return gain;
}

//##ModelId=3BD86E600283
void SourceBase::SetMinMaxGain(float min, float max) {
  alSourcef(sourcename_,AL_MIN_GAIN,min);
  alSourcef(sourcename_,AL_MAX_GAIN,max);
  ALenum error=alGetError();
  if(error!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError("Invalid value for min/max gain");
	break;
      default:
	throw FatalError("Error trying to set min/max gain!");
    }
}

//##ModelId=3BD86ECD01E0
void SourceBase::GetMinMaxGain(float & min, float &max) const {
  alGetSourcefv(sourcename_,AL_MIN_GAIN,&min);
  alGetSourcefv(sourcename_,AL_MAX_GAIN,&max);
}

//##ModelId=3BD86FB501BB
void SourceBase::SetAmbient(bool ambient) {
  if(ambient) {
    alSourcei(sourcename_,AL_SOURCE_RELATIVE,AL_TRUE);
    alSourcef(sourcename_,AL_ROLLOFF_FACTOR,0.0);
    MakeOmniDirectional();
  } else {
    alSourcei(sourcename_,AL_SOURCE_RELATIVE,AL_FALSE);
    alSourcef(sourcename_,AL_ROLLOFF_FACTOR,1.0);
  }
  if(alGetError()!=AL_FALSE)
    throw FatalError("Error trying to make/unmake sound ambient!");
}

//##ModelId=3BD8703E03D5
bool SourceBase::IsAmbient() const {
  ALint relative;
  alGetSourceiv(sourcename_,AL_SOURCE_RELATIVE,&relative);
  ALfloat rolloff;
  alGetSourcefv(sourcename_,AL_ROLLOFF_FACTOR,&rolloff);
  return (relative==AL_TRUE && rolloff<0.001);
}

//##ModelId=3BD8704C0078
void SourceBase::SetReferenceDistance(float distance) {
  alSourcef(sourcename_,AL_REFERENCE_DISTANCE,distance);
  if(alGetError()!=AL_FALSE)
    throw FatalError("Error trying to set reference distance!");
}

float SourceBase::GetReferenceDistance() const {
  ALfloat ref;
  alGetSourcefv(sourcename_,AL_REFERENCE_DISTANCE,&ref);
  return ref;
}

//##ModelId=3BD8708D0067
void SourceBase::SetMaxDistance(float distance) {
  alSourcef(sourcename_,AL_MAX_DISTANCE,distance);
  if(alGetError()!=AL_FALSE)
    throw FatalError("Error trying to set max distance!");
}

float SourceBase::GetMaxDistance() const {
  ALfloat maxdistance;
  alGetSourcefv(sourcename_,AL_MAX_DISTANCE,&maxdistance);
  return maxdistance;
}

//##ModelId=3BD870C700CF
void SourceBase::SetRolloffFactor(float factor) {
  alSourcef(sourcename_,AL_ROLLOFF_FACTOR,factor);
  if(alGetError()!=AL_FALSE)
    throw FatalError("Error trying to set rolloff factor!");
}

//##ModelId=3BD871130395
float SourceBase::GetRolloffFactor() const {
  ALfloat factor;
  alGetSourcefv(sourcename_,AL_ROLLOFF_FACTOR,&factor);
  return factor;
}

//##ModelId=3BD871230117
void SourceBase::SetPitch(float pitch) {
  alSourcef(sourcename_,AL_PITCH,pitch);
  ALenum error=alGetError();
  if(error!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError("Invalid value for pitch");
	break;
      default:
	throw FatalError("Error trying to set pitch!");
    }
}

// TODO: Remove/modify error throw.. :)
float SourceBase::GetPitch() const {
  ALfloat pitch;
  alGetSourcefv(sourcename_,AL_PITCH,&pitch);
  if(alGetError()!=AL_FALSE)
    throw FatalError("ARGH!");
  return pitch;
}

//##ModelId=3BD8716202C7
void SourceBase::SetReverbScale(float scale) throw (InitError,ValueError) {
  if(reverbinitiated_) {
    alReverbScale(sourcename_,scale);
    if(alGetError()!=AL_FALSE)
      throw ValueError("Reverb scale must be in range [0.0,1.0]");
    reverbscale_=scale;
  } else
    throw InitError("Reverb not initialized");
}

//##ModelId=3BD871D303B9
void SourceBase::SetReverbDelay(float delay) throw (InitError,ValueError) {
  if(reverbinitiated_) {
    alReverbDelay(sourcename_,delay);
    if(alGetError()!=AL_FALSE)
      throw ValueError("Reverb delay must be in range [0.0,2.0]");
    reverbdelay_=delay;
  } else
    throw InitError("Reverb not initialized");
}

float SourceBase::GetReverbScale() throw (InitError) {
  if(!reverbinitiated_)
    throw InitError("Reverb not initialized");
  return reverbscale_;
}

float SourceBase::GetReverbDelay() throw (InitError) {
  if(!reverbinitiated_)
    throw InitError("Reverb not initialized");
  return reverbdelay_;
}

//##ModelId=3BD8720F0239
ALuint SourceBase::Link(const SourceBase &source) throw (MemoryError) {
  if(nlinkedsources_>=alloclinkedsources_) {
    alloclinkedsources_+=5;
    linkedsources_=(ALuint *)realloc(linkedsources_,
				     sizeof(ALuint)*alloclinkedsources_);
    if(!linkedsources_)
      throw MemoryError("realloc failed");
  }
  linkedsources_[nlinkedsources_++]=source.GetAlSource();
  return source.GetAlSource();
}

//##ModelId=3BD872F6028B
void SourceBase::Unlink(const SourceBase &source) throw (NameError) {
  if(source.sourcename_==sourcename_)
    throw(NameError("Can't unlink a source from itself!"));
  for(unsigned int i=0;i<nlinkedsources_;i++)
    if(linkedsources_[i]==source.sourcename_) {
      linkedsources_[i]=linkedsources_[--nlinkedsources_];
      return;
    }
  throw NameError("Source not linked");
}

void SourceBase::Unlink(const ALuint name) throw (NameError) {
  if(name==sourcename_)
    throw(NameError("Can't unlink a source from itself!"));
  for(unsigned int i=0;i<nlinkedsources_;i++)
    if(linkedsources_[i]==name) {
      linkedsources_[i]=linkedsources_[--nlinkedsources_];
      return;
    }
  throw NameError("Source not linked");
}

//##ModelId=3BD87310027F
void SourceBase::UnlinkAll() {
  nlinkedsources_=1;
}

//##ModelId=3BD86F0102AD
ALuint SourceBase::GetAlSource() const {
  return sourcename_;
}

void SourceBase::SetPosition(float x, float y, float z) {
  alSource3f(sourcename_,AL_POSITION,x,y,z);
  ALenum error=alGetError();
  if(error!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError("Invalid value for position");
	break;
      default:
	throw FatalError("Error trying to set position!");
    }
}

void SourceBase::GetPosition(float &x, float &y, float &z) const {
  ALfloat position[3];
  alGetSourcefv(sourcename_,AL_POSITION,position);
  x=position[0];
  y=position[1];
  z=position[2];
}
  
void SourceBase::SetVelocity(float vx, float vy, float vz) {
  alSource3f(sourcename_,AL_VELOCITY,vx,vy,vz);
  ALenum error=alGetError();
  if(error!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError("Invalid value for velocity");
	break;
      default:
	throw FatalError("Error trying to set velocity!");
    }
}

void SourceBase::GetVelocity(float &vx, float &vy, float &vz) const {
  ALfloat velocity[3];
  alGetSourcefv(sourcename_,AL_VELOCITY,velocity);
  vx=velocity[0];
  vy=velocity[1];
  vz=velocity[2];
}

}
