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

#include "deviceupdater.h"

namespace openalpp {

static int RecordCallback(void *inputbuffer,void *outputbuffer,
			  unsigned long nframes,
			  PaTimestamp outtime, void *object) {
  // TODO: This is (probably) illegal to do in this callback. That might
  // be the explanation for the failures on small buffer sizes. Should
  // Implement an alternative solution that copies data and uses a semaphore
  // to signal that data has arrived..
  return (int)((DeviceUpdater *)object)->Update(inputbuffer,nframes*2);
}

DeviceUpdater::DeviceUpdater(int device, unsigned int frequency,unsigned int buffersize,
			     SampleFormat format,
			     ALuint buffer1,ALuint buffer2) 
			     :  StreamUpdater(buffer1,buffer2,AL_FORMAT_MONO8,frequency) {
  PaSampleFormat paformat;
  int nchannels;
  switch(format) {
    case(Mono8):
      format_=AL_FORMAT_MONO8;
      paformat=paInt8;
      nchannels=1;
      break;
    case(Mono16):
      format_=AL_FORMAT_MONO16;
      paformat=paInt16;
      nchannels=1;
      break;
    case(Stereo8):
      format_=AL_FORMAT_STEREO8;
      paformat=paInt8;
      nchannels=2;
      break;
    case(Stereo16):
      format_=AL_FORMAT_STEREO16;
      paformat=paInt16;
      nchannels=2;
      break;
  }

  PaDeviceID devid=paNoDevice;
  if(device<0)
    devid=Pa_GetDefaultInputDeviceID();
  else if(device<Pa_CountDevices()) {
    const PaDeviceInfo *devinfo=Pa_GetDeviceInfo((PaDeviceID)device);
    if(devinfo->maxInputChannels<nchannels)
      throw InitError("Requested number of channels not supported by device");
    devid=(PaDeviceID)device;
  } else
    throw InitError("No such device");
  if(devid==paNoDevice)
    throw InitError("Couldn't open device");  
  PaError err=Pa_OpenStream(&stream_,
			    devid,nchannels,paformat,NULL,
			    paNoDevice,0,paformat,NULL,
			    frequency,buffersize,2,paClipOff,
			    RecordCallback,this);
}

DeviceUpdater::~DeviceUpdater() {
  Pa_CloseStream(stream_);
}

void DeviceUpdater::Run() {
  Pa_StartStream(stream_);

  while(Pa_StreamActive(stream_))
    Pa_Sleep(1000);
}

}
