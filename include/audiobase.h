#ifndef AUDIOBASE_H_INCLUDED_C41996D8
#define AUDIOBASE_H_INCLUDED_C41996D8

#include <cstdlib>
extern "C" {
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>
}
#include "windowsstuff.h"
#include "error.h"

/**
 * \mainpage
 * This should be a short (?) text that describes OpenAL++
 */

/**
 * Namespace for OpenAL++.
 */
namespace openalpp {

/** 
 * Base class for enviroment, listener and source classes.
 * Takes care of initialisation/shutdown of anything necessary (e.g. ALut)
 */
class AudioBase {
  /**
   * Counter for #instances for enviroment, listener and source classes.
   * Used to determine when init and shutdown functions should be called
   */
  static int instances_;

  /**
   * Pointer to device.
   */
  static ALCdevice *device_;

  /**
   * Pointer to context.
   */
#ifndef WIN32
  static void *context_;
#else
  static struct ALCcontext_struct *context_;
#endif
 protected:
  /**
   * Constructor.
   * @param frequency is the output frequency, in Hz.
   * @param refresh is the refresh rate, in Hz.
   * @param is a flag for syncronous context. Values <0 indicates that the
   * default should be used.
   */
  AudioBase(int frequency=-1,int refresh=-1,int synchronous=-1)
    throw (InitError);

  /**
   * Destructor.
   */
  virtual ~AudioBase();

  /**
   * Flag for whether reverb has been initiated.
   * Reverb can be initiated with AudioEnviroment::InitiateReverb()
   */
  static bool reverbinitiated_;

  /**
   * Set reverb scale.
   * This pointer will be set by AudioEnviroment::InitiateReverb()
   * @param sid is the OpenAL name for the source
   * @param param is the reverb scale. Range [0.0,1.0].
   */
  static void (*alReverbScale)(ALuint sid, ALfloat param);

  /**
   * Set reverb delay.
   * This pointer will be set by AudioEnviroment::InitiateReverb()
   * @param sid is the OpenAL name for the source
   * @param param is the reverb delay. Range [0.0,2.0].
   */
  static void (*alReverbDelay)(ALuint sid, ALfloat param);
};

}

#endif /* AUDIOBASE_H_INCLUDED_C41996D8 */
