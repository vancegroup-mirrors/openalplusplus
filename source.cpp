#include "source.h"

namespace openalpp {

Source::Source(float x, float y, float z) : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=NULL;
}

Source::Source(const char *filename,float x,float y,float z)
  : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=new Sample(filename);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

Source::Source(const Sample &buffer,float x,float y,float z)
  : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=new Sample(buffer);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

Source::Source(const Stream &stream,float x,float y,float z) 
  : SourceBase(x,y,z) {
  streaming_=true;
  sounddata_=new Stream(stream);
  //  Don't need to buffer anything; Updater should handle queueing...
}

Source::Source(const Source &source) : SourceBase(source) {
  streaming_=source.streaming_;
  if(streaming_)
    sounddata_=new Stream((const Stream &)source.GetSound());
  else
    sounddata_=new Sample((const Sample &)source.GetSound());
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

Source::~Source() {
  delete sounddata_;
}

// TODO: Add flag for wether the sound should be loaded. This is useful for
// dummy sources that will be used only for groupsources.
void Source::SetSound(const char *filename) {
  streaming_=false;
  delete sounddata_;
  sounddata_=new Sample(filename);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

void Source::SetSound(const Sample &buffer) {
  streaming_=false;
  delete sounddata_;
  sounddata_=new Sample(buffer);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

void Source::SetSound(const Stream &stream) {
  streaming_=true;
  delete sounddata_;
  sounddata_=new Stream(stream);
}

const SoundData &Source::GetSound() const {
  return *sounddata_;
}

void Source::Play(const char *filename) {
  SetSound(filename);
  SourceBase::Play();
}

void Source::Play(const Sample &buffer) {
  SetSound(buffer);
  SourceBase::Play();
}

void Source::Play(const Stream &stream) {
  SetSound(stream);
  ((Stream *)sounddata_)->Record(sourcename_);
  SourceBase::Play();
}

void Source::Play() {
  if(streaming_)
    ((Stream *)sounddata_)->Record(sourcename_);
  SourceBase::Play();
}

void Source::Stop() {
  if(streaming_)
    ((Stream *)sounddata_)->Stop(sourcename_);
}

bool Source::IsStreaming() {
  return streaming_;
}

Source &Source::operator=(const Source &source) {
  if(this!=&source) {
    SourceBase::operator=(source);
    delete sounddata_;
    streaming_=source.streaming_;
    if(streaming_)
      sounddata_=new Stream(*(Stream *)source.sounddata_);
    else
      sounddata_=new Sample(*(Sample *)source.sounddata_);
  }
  return *this;
}

}
