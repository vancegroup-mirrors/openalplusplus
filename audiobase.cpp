#include "audiobase.h"

namespace openalpp {

AudioBase::AudioBase(int frequency,int refresh,int synchronous) 
  throw (InitError)
{
  if(!instances_) {
    device_=alcOpenDevice(NULL);
    if(!device_)
      throw InitError("Couldn't open device.");
    int attributes[7],i=0;
    attributes[0]=0;
    if(frequency>0) {
      attributes[i++]=ALC_FREQUENCY;
      attributes[i++]=frequency;
      attributes[i]=0;
    }
    if(refresh>0) {
      attributes[i++]=ALC_REFRESH;
      attributes[i++]=refresh;
      attributes[i]=0;
    }
    if(synchronous>0) {
      attributes[i++]=ALC_SYNC;
      attributes[i++]=synchronous;
      attributes[i]=0;
    }
    context_=alcCreateContext(device_,attributes);
    if(!context_ || alcGetError(device_)!=AL_FALSE) {
      if(context_)
	alcDestroyContext(context_);
      else
	alcCloseDevice(device_);
      throw InitError("Couldn't create context.");
    }    
    alcMakeContextCurrent(context_);
    reverbinitiated_=false;
  }
  instances_++;
}

AudioBase::~AudioBase() {
  instances_--;
  if(!instances_) {
    alcDestroyContext(context_);
  }
}

// Static member
int AudioBase::instances_=0;

}
