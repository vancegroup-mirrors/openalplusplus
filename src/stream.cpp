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
