#include "streamupdater.h"

namespace openalpp {

StreamUpdater::StreamUpdater(ALuint buffer1,ALuint buffer2,
			     ALenum format,unsigned int frequency) 
  : format_(format), frequency_(frequency), stoprunning_(false) {
  buffers_[0]=buffer1;
  buffers_[1]=buffer2;
  nrefs_=1;
  setCancel(ost::THREAD_CANCEL_DISABLED);
}

StreamUpdater::~StreamUpdater() {
  Terminate();
}

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
  alSourceStop(sourcename);
  alSourceUnqueueBuffers(sourcename,2,buffers_);
  alGetError();
  LEAVE_CRITICAL;
}

bool StreamUpdater::Update(void *buffer,unsigned int length) {
  ALint processed,nprocessed,state,nstate;
  ALuint albuffer;

  ENTER_CRITICAL;

  while(sources_.size() && removesources_.size()>0) {
    for(unsigned int i=0;i<sources_.size();i++)
      if(removesources_.back()==sources_[i] && (i+1)<sources_.size()) {
	sources_[i]=sources_[i+1];
	break;
      }
    sources_.pop_back();
    removesources_.pop_back();
  }

  while(newsources_.size() || !sources_.size()) // Add any new sources
    if(newsources_.size()) {
      sources_.push_back(newsources_.back());
      alSourceQueueBuffers(newsources_.back(),2,buffers_);
      alSourceStop(newsources_.back());
      newsources_.pop_back();
    } else {
      LEAVE_CRITICAL;
      Sleep(50);
      ENTER_CRITICAL;
    }

  processed=0;
  while(!processed) {
    state=AL_PLAYING;
    for(unsigned int i=0;i<sources_.size();i++) {
      alGetSourceiv(sources_[i],AL_SOURCE_STATE,&nstate);
      state&=nstate;
    }

    if(state!=AL_PLAYING) {
      for(unsigned int i=0;i<sources_.size();i++) {
	alSourceStop(sources_[i]);
        alSourceUnqueueBuffers(sources_[i],2,buffers_);
      }
      alBufferData(buffers_[0],format_,buffer,length/2,frequency_);
      alBufferData(buffers_[1],format_,
		   (char *)buffer+length/2,length/2,frequency_);
      for(unsigned int i=0;i<sources_.size();i++) {
        alSourceQueueBuffers(sources_[i],2,buffers_);
	alSourcePlay(sources_[i]);  // TODO: This would be better handled by
	                 // alSourcePlayv((ALuint *)&sources_[0]..)...;
      }
    } else {
      processed=1;
      for(unsigned int i=0;i<sources_.size();i++) {
        alGetSourceiv(sources_[i],AL_BUFFERS_PROCESSED,&nprocessed);
	processed=processed && nprocessed;
	if(!processed)
	  break;
      }
      if(processed) {
	for(unsigned int i=0;i<sources_.size();i++)
          alSourceUnqueueBuffers(sources_[i],1,&albuffer);
        alBufferData(albuffer,format_,buffer,length,frequency_);
	for(unsigned int i=0;i<sources_.size();i++)
          alSourceQueueBuffers(sources_[i],1,&albuffer);
      } else {
	LEAVE_CRITICAL;
        Sleep(50);
	ENTER_CRITICAL;
	if(removesources_.size()) { // Not sure if this is necessary, but just in case...
	  LEAVE_CRITICAL;
	  return Update(buffer,length);
	}
      }
    }
  }
/*
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
*/  
  LEAVE_CRITICAL;
  bool ret;
  runmutex_.EnterMutex();
  ret=stoprunning_;
  runmutex_.LeaveMutex();
  return ret;
}

void StreamUpdater::Final() {
  delete this;
} 

StreamUpdater *StreamUpdater::Reference() {
  nrefs_++;
  return this;
}

void StreamUpdater::DeReference() throw (FatalError) {
  nrefs_--;
  if(!nrefs_) {
    runmutex_.EnterMutex();
    stoprunning_=true;
    runmutex_.LeaveMutex();
  } else if(nrefs_<0)
    throw FatalError("StreamUpdater dereferenced too many times!");
}

}
