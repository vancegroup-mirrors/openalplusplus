#include "audioenviroment.h"

namespace openalpp {

AudioEnviroment::AudioEnviroment() throw (InitError): AudioBase() {
}

AudioEnviroment::AudioEnviroment(int frequency,int refresh,bool synchronous)
  throw (InitError)
  : AudioBase(frequency,refresh,synchronous) {
}

AudioEnviroment::AudioEnviroment(int frequency,int refresh) throw (InitError)
  : AudioBase(frequency,refresh) {
}

void AudioEnviroment::SetSoundSpeed(float speed) throw (ValueError,FatalError){
  alDopplerVelocity(speed);
  ALenum error;
  if((error=alGetError())!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError((const char *)alGetString(error));
	break;
      default:
	throw FatalError("Unknown error in AudioEnviroment::SetSoundSpeed()");
	break;
    }
}

float AudioEnviroment::GetSoundSpeed() throw (FatalError) {
  ALfloat speed;
  alGetFloatv(AL_DOPPLER_VELOCITY,&speed);
  if(alGetError()!=AL_FALSE)  // This isn't strictly necessary...
    throw FatalError("Unknown error in AudioEnviroment::GetSoundSpeed()");
  return speed;
}

void SetDopplerFactor(float factor) throw (ValueError,FatalError) {
  alDopplerFactor(factor);
  ALenum error;
  if((error=alGetError())!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError((const char *)alGetString(error));
	break;
      default:
	throw FatalError("Unknown error in AudioEnviroment::SetDopplerFactor()");
	break;
    }
}

float GetDopplerFactor() throw (FatalError) {
  ALfloat(factor);
  alGetFloatv(AL_DOPPLER_FACTOR,&factor);
  if(alGetError()!=AL_FALSE)  // This isn't strictly necessary...
    throw FatalError("Unknown error in AudioEnviroment::GetDopplerFactor()");
  return factor;
}

void AudioEnviroment::SetGain(float gain) {
  alListenerf(AL_GAIN,gain);
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

float AudioEnviroment::GetGain() throw (FatalError) {  
  ALfloat gain;
  alGetListenerfv(AL_GAIN,&gain);
  if(alGetError()!=AL_FALSE)          // This isn't strictly necessary...
    throw FatalError("Unknown error in AudioEnviroment::GetGain()");
  return gain;
}

void AudioEnviroment::SetDistanceModel(DistanceModel model) throw (FatalError){
  switch(model) {
    case(None):
      alDistanceModel(AL_NONE);
      break;
    case(InverseDistance):
      alDistanceModel(AL_INVERSE_DISTANCE);
      break;
    case(InverseDistanceClamped):
      alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
      break;
    default:
      throw FatalError("Unknown model in AudioEnviroment::SetDistanceModel()");
  }
  if(alGetError()!=AL_FALSE)
    throw FatalError("alDistanceModel failed in AudioEnviroment::SetDistanceModel()");
}

DistanceModel AudioEnviroment::GetDistanceModel() throw (FatalError) {
  int almodel;
  alGetIntegerv(AL_DISTANCE_MODEL,&almodel);
  switch(almodel) {
    case(AL_NONE):
      return None;
    case(AL_INVERSE_DISTANCE):
      return InverseDistance;
    case(AL_INVERSE_DISTANCE_CLAMPED):
      return InverseDistanceClamped;
    default:
      throw FatalError("Unknown distance model in AudioEnviroment::GetDistanceModel");
  }
}

// Static fields in AudioBase
bool AudioBase::reverbinitiated_;
void (*AudioBase::alReverbScale)(ALuint sid, ALfloat param);
void (*AudioBase::alReverbDelay)(ALuint sid, ALfloat param);

void AudioEnviroment::InitiateReverb() throw (InitError) {
  alReverbScale=(void (*)(ALuint sid, ALfloat param))
    alGetProcAddress((ALubyte *)"alReverbScale_LOKI");
  alReverbDelay=(void (*)(ALuint sid, ALfloat param))
    alGetProcAddress((ALubyte *)"alReverbDelay_LOKI");
  if(!(alReverbScale && alReverbDelay)) {
    throw InitError("Couldn't initiate reverb");
  } else
    reverbinitiated_=true;
}


}
