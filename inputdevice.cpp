#include "inputdevice.h"

namespace openalpp {

// Static
int InputDevice::nobjects_=0;

void InputDevice::Init() {
  if(!nobjects_) {
    PaError err=Pa_Initialize();
    if(err!=paNoError)
      throw InitError("Error initializing PortAudio");
  }
  nobjects_++;
}

InputDevice::InputDevice() {
  Init();
  updater_=new DeviceUpdater(-1,44100,10000,Mono16,buffername_,buffer2_->GetName());
}

InputDevice::InputDevice(int device,unsigned int samplerate,unsigned int buffersize,
			 SampleFormat format) {
  Init();

  updater_=new DeviceUpdater(device,samplerate,buffersize,format,buffername_,buffer2_->GetName());
}

InputDevice::InputDevice(const InputDevice &input)
  : Stream((const Stream &)input) {
}

InputDevice &InputDevice::operator=(const InputDevice &input) {
  if(this!=&input) {
    Stream::operator=((const Stream &)input);
  }
  return *this;
}

InputDevice::~InputDevice() {
  nobjects_--;
  if(!nobjects_)
    Pa_Terminate();
}

}
