#include "sounddata.h"

namespace openalpp {

SoundData::SoundBuffer::SoundBuffer() throw (NameError) {
  alGenBuffers(1,&buffername_);
  if(alGetError()!=AL_FALSE)
    throw NameError("Error generating buffer name");
  refcount_=1;
}

SoundData::SoundBuffer::~SoundBuffer() {
  alDeleteBuffers(1,&buffername_);
}

SoundData::SoundBuffer *SoundData::SoundBuffer::Reference() {
  refcount_++;
  return this;
}

void SoundData::SoundBuffer::DeReference() throw (FatalError) {
  refcount_--;
  if(!refcount_)
    delete this;
  else if(refcount_<0)
    throw FatalError("SoundBuffer dereferenced too many times!");
}



SoundData::SoundData() throw (NameError,InitError) : AudioBase() {
  buffer_=new SoundBuffer();
  buffername_=buffer_->GetName();
}

SoundData::SoundData(const SoundData &sounddata) {
  buffer_=sounddata.buffer_->Reference();
  buffername_=buffer_->GetName();
}

SoundData::~SoundData() {
  buffer_->DeReference();
}

ALuint SoundData::GetAlBuffer() const {
  return buffername_;
}

SoundData &SoundData::operator=(const SoundData &sounddata) {
  if(this!=&sounddata) {
    buffer_->DeReference();
    buffer_=sounddata.buffer_->Reference();
    buffername_=buffer_->GetName();
  }
  return *this;
}

}
