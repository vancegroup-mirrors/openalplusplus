#include "stream.h"

namespace openalpp {

Stream::Stream() throw (NameError) : SoundData() {
  buffer2_=new SoundBuffer();
  updater_=NULL;
}

Stream::Stream(const Stream &stream) : SoundData((const SoundData &)stream) {
  buffer2_=stream.buffer2_->Reference();
  updater_=stream.updater_->Reference();
}

Stream &Stream::operator=(const Stream &stream) {
  if(this!=&stream) {
    SoundData::operator=((const SoundData &)stream);
    buffer2_->DeReference();
    buffer2_=stream.buffer2_->Reference();
    updater_->DeReference();
    updater_=stream.updater_->Reference();
  }
  return *this;
}

Stream::~Stream() {
  buffer2_->DeReference();
  updater_->DeReference();
}

void Stream::Record(ALuint sourcename) {
  if(!updater_)
    throw FatalError("No updater thread for stream!");
  updater_->AddSource(sourcename);
}

void Stream::Stop(ALuint sourcename) {
  if(!updater_)
    throw FatalError("No updater thread for stream!");
  updater_->RemoveSource(sourcename);
}

}
