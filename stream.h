#ifndef STREAM_H_INCLUDED_C41983DF
#define STREAM_H_INCLUDED_C41983DF
#include "sounddata.h"

namespace openalpp {

/**
 * Base class for NetStream and InputDevice.
 * Used for audio streams.
 */
class Stream : public SoundData{
 public:
  /**
   * Start recording.
   * I.e. start copying data to buffers.
   */
  void Record();

  /**
   * Stop recording.
   */
  void Stop();
};

}

#endif /* STREAM_H_INCLUDED_C41983DF */
