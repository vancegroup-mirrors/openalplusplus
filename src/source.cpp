/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Umeå University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
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

#include "openalpp/source.h"

using namespace openalpp;

Source::Source(float x, float y, float z) : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=NULL;
}

Source::Source(const char *filename,float x,float y,float z)
  : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=new Sample(filename);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

Source::Source(const Sample &buffer,float x,float y,float z)
  : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=new Sample(buffer);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
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
    sounddata_=new Stream((const Stream &)source.getSound());
  else
    sounddata_=new Sample((const Sample &)source.getSound());
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

Source::~Source() {
  stop();
  if (sounddata_)
    delete sounddata_;

  sounddata_ = 0L;
}

// TODO: Add flag for wether the sound should be loaded. This is useful for
// dummy sources that will be used only for groupsources.
void Source::setSound(const std::string& filename) {
  streaming_=false;
  if (sounddata_)
    delete sounddata_;
  sounddata_=new Sample(filename);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

void Source::setSound(const Sample &buffer) {
  streaming_=false;
  if (sounddata_)
    delete sounddata_;
  sounddata_=new Sample(buffer);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

void Source::setSound(const Stream &stream) {
  streaming_=true;
  if (sounddata_)
    delete sounddata_;
  sounddata_=new Stream(stream);
}

const SoundData &Source::getSound() const {
  return *sounddata_;
}

void Source::play(const std::string& filename) {
  setSound(filename.c_str());
  SourceBase::play();
}

void Source::play(const Sample &buffer) {
  setSound(buffer);
  SourceBase::play();
}

void Source::play(const Stream &stream) {
  alSourcei(sourcename_,AL_LOOPING,AL_FALSE); //Streaming sources can't loop...
  setSound(stream);
  ((Stream *)sounddata_)->record(sourcename_);
  SourceBase::play();
}

void Source::play() {
  if(streaming_) {
    alSourcei(sourcename_,AL_LOOPING,AL_FALSE); //Streaming sources can't loop...
    ((Stream *)sounddata_)->record(sourcename_);
  }
  SourceBase::play();
}

void Source::stop() {
  if(streaming_)
    ((Stream *)sounddata_)->stop(sourcename_);
  SourceBase::stop();
}

void Source::pause() {
  SourceBase::pause();
}


bool Source::isStreaming() {
  return streaming_;
}

Source &Source::operator=(const Source &source) {
  if(this!=&source) {
    SourceBase::operator=(source);
    if (sounddata_)
      delete sounddata_;
    streaming_=source.streaming_;
    if(streaming_)
      sounddata_=new Stream(*(Stream *)source.sounddata_);
    else
      sounddata_=new Sample(*(Sample *)source.sounddata_);
  }
  return *this;
}

