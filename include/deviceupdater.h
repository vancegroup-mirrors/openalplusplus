#ifndef DEVICEUPDATER_H_INCLUDED_C419EA9C
#define DEVICEUPDATER_H_INCLUDED_C419EA9C

#include "streamupdater.h"
#include "error.h"
#include <portaudio.h>

namespace openalpp {

/**
 * Updater for sound input devices (like microphones).
 */
class DeviceUpdater : public StreamUpdater {
  PortAudioStream *stream_;
 public:
  /**
   * Constructor.
   * @param device is the number of the device (as PortAudio reports it).
   * TODO: Change this to something non-portaudio dependant.
   * @param frequency is the desired recording frequency.
   * @param buffersize is the desired buffersize.
   * @param format is the format of the sound.
   * @param buffer1 and..
   * @param buffer2 are the (OpenAL) buffers to use.
   */
  DeviceUpdater(int device, unsigned int frequency,unsigned int buffersize,
		SampleFormat format,
		ALuint buffer1,ALuint buffer2);

  /**
   * Destructor.
   */
  ~DeviceUpdater();

  /**
   * Inherited from Thread.
   * This will be called when the updater is Start():ed..
   */
  void Run();
};

}

#endif /* DEVICEUPDATER_H_INCLUDED_C419EA9C */
