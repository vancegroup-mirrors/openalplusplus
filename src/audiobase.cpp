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

#include "audiobase.h"

namespace openalpp {

AudioBase::AudioBase(int frequency,int refresh,int synchronous) 
  throw (InitError)
{
  if(!instances_) {
    // Open a write (output) device. This should (in theory) make it possible
    // to open a read (input) device later.. 
    device_=alcOpenDevice((/*const */ALubyte *)"'((direction \"write\"))");
    if(!device_)
      throw InitError("Couldn't open device.");
    int attributes[7],i=0;
    attributes[0]=0;
    if(frequency>0) {
      attributes[i++]=ALC_FREQUENCY;
      attributes[i++]=frequency;
      attributes[i]=0;
    }
    if(refresh>0) {
      attributes[i++]=ALC_REFRESH;
      attributes[i++]=refresh;
      attributes[i]=0;
    }
    if(synchronous>0) {
      attributes[i++]=ALC_SYNC;
      attributes[i++]=synchronous;
      attributes[i]=0;
    }
    context_=alcCreateContext(device_,attributes);
    if(!context_ || alcGetError(device_)!=ALC_NO_ERROR) {
      if(context_)
	alcDestroyContext(context_);
      alcCloseDevice(device_);
      throw InitError("Couldn't create context");
    } 
    alcMakeContextCurrent(context_);
    reverbinitiated_=false;
  }
  instances_++;
}

AudioBase::~AudioBase() {
  instances_--;
  if(!instances_) {
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context_);
    alcCloseDevice(device_);
  }
}

// Static members
int AudioBase::instances_=0;
ALCdevice *AudioBase::device_=NULL;
#ifndef WIN32
void *AudioBase::context_=NULL;
#else
struct ALCcontext_struct *AudioBase::context_=NULL;
#endif

}
