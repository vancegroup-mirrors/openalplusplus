#include "deviceupdater.h"

namespace openalpp {

static int RecordCallback(void *inputbuffer,void *outputbuffer,
			  unsigned long nframes,
			  PaTimestamp outtime, void *object) {
  ((DeviceUpdater *)object)->Update(inputbuffer,nframes*2);

  return 0;
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
