#ifndef STREAMUPDATER_H_INCLUDED_C419FA12
#define STREAMUPDATER_H_INCLUDED_C419FA12

#include <cc++/thread.h>
#include <vector>
extern "C" {
#include <AL/al.h>
}
#include "error.h"
#include "windowsstuff.h"

namespace openalpp {

/**
 * Format for sound data. Mono/Stereo, 8 or 16 bits.
 */
typedef enum SampleFormat {Mono8,Stereo8,Mono16,Stereo16};

/**
 * Base class for (threaded) updating of stream buffers.
 */
class StreamUpdater : public ost::Thread,ost::Mutex {
  /**
   * #references to this instance.
   * If zero => delete this.
   */
  int nrefs_;
 public:
  /**
   * Constructor.
   * @param buffer1 and...
   * @param buffer2 are the buffers used for double-buffered streaming.
   * @param format is the (OpenAL) format of the sound.
   * @param frequency is the frequency of the sound.
   */
  StreamUpdater(ALuint buffer1,ALuint buffer2,
		ALenum format,unsigned int frequency);

  /**
   * Destructor.
   */
  ~StreamUpdater();

  /**
   * Add a source to the stream.
   * @param sourcename is the OpenAL name of the source.
   */
  void AddSource(ALuint sourcename);

  /**
   * Remove a source from the stream.
   * @param sourcename is the OpenAL name of the source.
   */
  void RemoveSource(ALuint sourcename);

  /**
   * Update the stream.
   * I.e. add new data to play.
   * @param buffer is a pointer to sound data.
   * @param length is the length of the sound data (in bytes).
   */
  void Update(void *buffer,unsigned int length); 

  /**
   * Inherited from Thread.
   * Is called after Run() finishes, and deletes this.
   */
  void Final();

  /**
   * Reference this updater.
   * @return this.
   */
  StreamUpdater *Reference();

  /**
   * Dereference this updater.
   */
  void DeReference() throw (FatalError);
 protected:
  /**
   * Names of the buffers to update.
   */
  ALuint buffers_[2];

  /**
   * OpenAL format of the sound data.
   */
  ALenum format_;

  /**
   * Frequency of the sound data.
   */
  unsigned int frequency_;

  /**
   * Source to update.
   */
  std::vector<ALuint> sources_,newsources_,removesources_;

  /**
   * Flag for when Run should stop running..
   */ 
  bool stoprunning_;

  /**
   * Mutex for stoprunning_.
   */
  ost::Mutex runmutex_;

};

}

#endif /* STREAMUPDATER_H_INCLUDED_C419FA12 */
