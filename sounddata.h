#ifndef SOUNDDATA_H_INCLUDED_C4199A75
#define SOUNDDATA_H_INCLUDED_C4199A75

#include "error.h"
extern "C" {
#include <AL/al.h>
}

namespace openalpp {

/**
 * Base class for sound data.
 */
class SoundData {
  /**
   * Private class to handle generation/deletion of OpenAL buffers correctly.
   */
  class SoundBuffer {
    ALuint buffername_;
    int refcount_;
  public:
    SoundBuffer() throw (NameError);
    ~SoundBuffer();
    SoundBuffer *Reference();
    void DeReference() throw (FatalError);
    ALuint GetName() {return buffername_;}
  };
 public:
  /**
   * Get the OpenAL name for the buffer.
   * @return the OpenAL name.
   */
  ALuint GetAlBuffer() const;

  /**
   * Constructor.
   */
  SoundData() throw (NameError);

  /**
   * Copy constructor.
   */
  SoundData(const SoundData &sounddata);

  /**
   * Destructor.
   */
  ~SoundData();

  /**
   * Assignment operator.
   */
  SoundData &operator=(const SoundData &sounddata);
 protected:
  /**
   * See class SoundBuffer comment.
   */
  SoundBuffer *buffer_;

  /**
   * OpenAL name for the buffer.
   */
  ALuint buffername_;
};

}

#endif /* SOUNDDATA_H_INCLUDED_C4199A75 */
