/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Umeå University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *              CommonC++ (http://cplusplus.sourceforge.net/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

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
