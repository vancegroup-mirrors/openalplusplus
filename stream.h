#ifndef STREAM_H_INCLUDED_C41983DF
#define STREAM_H_INCLUDED_C41983DF
#include "streamupdater.h"
#include "sounddata.h"

namespace openalpp {

/**
 * Base class for NetStream and InputDevice.
 * Used for audio streams.
 */
class Stream : public SoundData {
 protected:
  /**
   * For double-buffering of sounds.
   */
  SoundBuffer *buffer2_;

  StreamUpdater *updater_;
 public:
  /**
   * Default constructor.
   */
  Stream() throw (NameError);

  /**
   * Copy constructor.
   */
  Stream(const Stream &stream);

  /**
   * Assignment operator.
   */
  Stream &operator=(const Stream &stream);

  /**
   * Destructor.
   */
  ~Stream();

  /**
   * Start recording.
   * I.e. start copying data to buffers.
   * @param sourcename is the (OpenAL) name of the source.
   */
  void Record(ALuint sourcename);

  /**
   * Stop recording.
   * @param sourcename is the (OpenAL) name of the source.
   */
  void Stop(ALuint sourcename);
};

}

#endif /* STREAM_H_INCLUDED_C41983DF */
