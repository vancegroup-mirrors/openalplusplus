#include "source.h"

namespace openalpp {

//##ModelId=3BD876CF0075
Source::Source(float x, float y, float z) : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=NULL;
}

//##ModelId=3BD876DB013A
Source::Source(const char *filename,float x,float y,float z)
  : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=new Sample(filename);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

//##ModelId=3BD876FC032D
Source::Source(const Sample &buffer,float x,float y,float z)
  : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=new Sample(buffer);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

//##ModelId=3BD8772C029F
Source::Source(const Stream &stream,float x,float y,float z) 
  : SourceBase(x,y,z) {
  streaming_=true;
  // TODO: Streaming!!!
}

//##ModelId=3BDD2E5D01FA
Source::Source(const Source &source) : SourceBase(source) {
  streaming_=source.streaming_;
  if(streaming_)
    ; // TODO: Implement streaming!!!
  else
    sounddata_=new Sample((const Sample &)source.GetSound());
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

//##ModelId=3BDD28F500B0
Source::~Source() {
  delete sounddata_;
}

//##ModelId=3BDEBF9901DA
// TODO: Add flag for wether the sound should be loaded. This is useful for
// dummy sources that will be used only for groupsources.
void Source::SetSound(const char *filename) {
  streaming_=false;
  delete sounddata_;
  sounddata_=new Sample(filename);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

//##ModelId=3BD877BC0062
void Source::SetSound(const Sample &buffer) {
  streaming_=false;
  delete sounddata_;
  sounddata_=new Sample(buffer);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->GetAlBuffer());
}

//##ModelId=3BDD31AF02ED
void Source::SetSound(const Stream &stream) {
  streaming_=true;
  // TODO: Streaming !!!
}

//##ModelId=3BD8779500C0
const SoundData &Source::GetSound() const {
  return *sounddata_;
}

//##ModelId=3BD8781B0393
void Source::Play(const char *filename) {
  SetSound(filename);
  Play();
}

//##ModelId=3BD87846027D
void Source::Play(const Sample &buffer) {
  SetSound(buffer);
  Play();
}

//##ModelId=3BD87875009A
void Source::Play(const Stream &stream) {
  SetSound(stream);
  Play();
}

void Source::Play() {
  SourceBase::Play();
}

//##ModelId=3BD8788F00C9
bool Source::IsStreaming() {
  return streaming_;
}

//##ModelId=3BDD2E7100D7
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
