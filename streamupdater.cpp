#include "streamupdater.h"

namespace openalpp {

StreamUpdater::StreamUpdater(ALuint buffer1,ALuint buffer2,
			     ALenum format,unsigned int frequency) 
  : format_(format), frequency_(frequency) {
  buffers_[0]=buffer1;
  buffers_[1]=buffer2;
  nrefs_=1;
}

// TODO: Neither AddSource nor RemoveSource work as they should now...
void StreamUpdater::AddSource(ALuint sourcename) {
  ENTER_CRITICAL;
  newsources_.push_back(sourcename);
  if(!sources_.size())
    Start();
  LEAVE_CRITICAL;
}

void StreamUpdater::RemoveSource(ALuint sourcename) {
  ENTER_CRITICAL;
  removesources_.push_back(sourcename);
  LEAVE_CRITICAL;
}

void StreamUpdater::Update(void *buffer,unsigned int length) {
  ALint processed,state;
  ALuint albuffer;

  ENTER_CRITICAL;

  // TODO: Remove sources here. Must make sure to unqueue all buffers

  while(newsources_.size() || !sources_.size()) // Add any new sources
    if(newsources_.size()) {
      sources_.push_back(newsources_.back());
      newsources_.pop_back();
    } else {
      LEAVE_CRITICAL;
      Sleep(50);
      ENTER_CRITICAL;
    }

  processed=0;
  while(!processed) {
    alGetSourceiv(sources_[0],AL_SOURCE_STATE,&state);
    if(state!=AL_PLAYING) {
      alBufferData(buffers_[0],format_,buffer,length/2,frequency_);
      alBufferData(buffers_[1],format_,
		   (char *)buffer+length/2,length/2,frequency_);
      alSourceQueueBuffers(sources_[0],2,buffers_);
      alSourcePlay(sources_[0]);
      break;
    }
    alGetSourceiv(sources_[0],AL_BUFFERS_PROCESSED,&processed);
    if(processed) {
      alSourceUnqueueBuffers(sources_[0],1,&albuffer);
      alBufferData(albuffer,format_,buffer,length,frequency_);
      alSourceQueueBuffers(sources_[0],1,&albuffer);
    } else
      Sleep(50);
  }
  LEAVE_CRITICAL;
}

StreamUpdater *StreamUpdater::Reference() {
  nrefs_++;
  return this;
}

void StreamUpdater::DeReference() throw (FatalError) {
  nrefs_--;
  if(!nrefs_) {
    Exit();
    delete this;
  } else if(nrefs_<0)
    throw FatalError("StreamUpdater dereferenced too many times!");
}

}
