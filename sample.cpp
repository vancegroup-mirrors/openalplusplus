#include "sample.h"

namespace openalpp {

Sample::Sample(const char *filename) throw (FileError)
  : SoundData(),filename_(filename) {
  ALsizei size,bits,freq;
  ALenum format,error;
  ALvoid *data;
  ALboolean success;

  success=alutLoadWAV(filename,&data,&format,&size,&bits,&freq);
  if(success==AL_TRUE) {
    alBufferData(buffername_,format,data,size,freq);
    if((error=alGetError())!=AL_FALSE)
      throw FileError("Error buffering sound");
    free(data);
  } else 
    throw FileError("Unable to load file");
}

Sample::Sample(const Sample &sample)
  : SoundData((const SoundData &)sample), filename_(sample.filename_) {
}

std::string Sample::GetFileName() const {
  return filename_;
}

Sample &Sample::operator=(const Sample &sample) {
  if(this!=&sample) {
    SoundData::operator=(sample);
    filename_=sample.filename_;
  }
  return *this;
}

}
