#include "streamupdater.h"

namespace openalpp {

StreamUpdater::StreamUpdater(ALuint buffer1,ALuint buffer2,
			     ALenum format,unsigned int frequency) 
  : format_(format), frequency_(frequency), source_(0) {
  buffers_[0]=buffer1;
  buffers_[1]=buffer2;
  nrefs_=1;
}

// TODO: Neither AddSource nor RemoveSource work as they should now...
void StreamUpdater::AddSource(ALuint sourcename) {
  source_=sourcename;
}

void StreamUpdater::RemoveSource(ALuint sourcename) {
}

void StreamUpdater::Update(void *buffer,unsigned int length) {
  ALint processed,state;
  ALuint albuffer;

  // Lock mutex (?)
  processed=0;
  while(!processed) {
    alGetSourceiv(source_,AL_SOURCE_STATE,&state);
    if(state!=AL_PLAYING) {
      alBufferData(buffers_[0],format_,buffer,length/2,frequency_);
      alBufferData(buffers_[1],format_,
		   (char *)buffer+length/2,length/2,frequency_);
      alSourceQueueBuffers(source_,2,buffers_);
      alSourcePlay(source_);
      break;
    }
    alGetSourceiv(source_,AL_BUFFERS_PROCESSED,&processed);
    if(processed) {
      alSourceUnqueueBuffers(source_,1,&albuffer);
      alBufferData(albuffer,format_,buffer,length,frequency_);
      alSourceQueueBuffers(source_,1,&albuffer);
    } else
      Sleep(50);
  }
  // Unlock mutex?
}

StreamUpdater *StreamUpdater::Reference() {
  nrefs_++;
  return this;
}

void StreamUpdater::DeReference() throw (FatalError) {
  nrefs_--;
  if(!nrefs_)
    delete this;
  else if(nrefs_<0)
    throw FatalError("StreamUpdater dereferenced too many times!");
}

}
