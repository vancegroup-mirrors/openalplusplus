#ifndef INPUTDEVICE_H_INCLUDED_C41987DC
#define INPUTDEVICE_H_INCLUDED_C41987DC

#include "deviceupdater.h"
#include "stream.h"
#include <portaudio.h>

namespace openalpp {

/**
 * Class for handling input devices, like microphones.
 */
class InputDevice : public Stream {
  static int nobjects_;

  /**
   * Initialize input. Called by constructor(s).
   */
  void Init();
 public:
  /**
   * Constructor.
   */
  InputDevice();

  /**
   * Constructor.
   * @param device is the device to open. -1 for default input.
   * @param samplerate is the desired sample rate.
   * @param buffersize is the desired buffer size.
   * @param format is the desired sample format.
   */
  InputDevice(int device,unsigned int samplerate,unsigned int buffersize=1024, 
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
};

}

#endif /* INPUTDEVICE_H_INCLUDED_C41987DC */
