#ifndef INPUTDEVICE_H_INCLUDED_C41987DC
#define INPUTDEVICE_H_INCLUDED_C41987DC

#include "deviceupdater.h"
#include "stream.h"

namespace openalpp {

/**
 * Class for handling input devices, like microphones.
 */
class InputDevice : public Stream {
 public:
  /**
   * Constructor.
   */
  InputDevice();

  /**
   * Constructor.
   * @param samplerate is the desired sample rate.
   * @param buffersize is the desired buffer size.
   * @param format is the desired sample format.
   */
  InputDevice(unsigned int samplerate,unsigned int buffersize=1024, 
	      SampleFormat format=Mono16);

  /**
   * Copy constructor.
   */
  InputDevice(const InputDevice &input);

  /**
   * Assignment operator.
   */
  InputDevice &operator=(const InputDevice &input);

  /**
   * Destructor.
   */
  ~InputDevice();

 private:
  /**
   * Updater that runs in its own thread.
   */
  DeviceUpdater *updater_;
};

}

#endif /* INPUTDEVICE_H_INCLUDED_C41987DC */
