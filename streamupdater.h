#ifndef STREAMUPDATER_H_INCLUDED_C419FA12
#define STREAMUPDATER_H_INCLUDED_C419FA12

#include <cc++/thread.h>
#include <AL/al.h>

namespace openalpp {

/**
 * Base class for (threaded) updating of stream buffers.
 */
#ifdef WIN32
class StreamUpdater : public ost::Thread {
#else
class StreamUpdater : public Thread {
#endif
 public:
  /**
   * Constructor.
   * @param buffer is the buffer to update.
   */
  StreamUpdater(ALuint buffer);

  /**
   * Dereference this instance.
   */
  void DeReference();

  /**
   * Reference this instance.
   * @return a pointer to this instance.
   */
  StreamUpdater *Reference();

 protected:
  /**
   * 
   */
  void Update();

  /**
   * Name of the buffer to update.
   */
  ALuint buffername_;
};

}

#endif /* STREAMUPDATER_H_INCLUDED_C419FA12 */
