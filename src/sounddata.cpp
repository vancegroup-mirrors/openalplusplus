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

SoundData::SoundData(const SoundData &sounddata)
  : AudioBase(/*(const AudioBase &)sounddata*/) {
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
