#ifndef AUDIOENVIROMENT_H_INCLUDED_C4211030
#define AUDIOENVIROMENT_H_INCLUDED_C4211030

#include "audiobase.h"
#include "error.h"

namespace openalpp {

/**
 * Enum for setting/getting the current distance model.
 * None means no model is in use, i.e. no attenuation.\n
 * InverseDistance:\n
 * - \f$G=GAIN-20*log_{10}*(1+ROLLOFF*\frac{dist-REFERENCE\_DISTANCE}{REFERENCE\_DISTANCE})\f$
 * - \f$G=min (G,MAX\_GAIN)\f$
 * - \f$G=max (G,MIN\_GAIN)\f$
 *
 * InverseDistanceClamped:\n
 * - \f$dist=max (dist,REFERENCE\_DISTANCE)\f$
 * - \f$dist=min (dist,MAX\_DISTANCE)\f$
 * - And then the calculations in InverseDistance... This is equivalent to the
 * <a href="http://www.iasig.org>IASIG</a> I3DL2 distance model.\n
 *
 * In the above calculations, the variables have the following meanings:\n
 * - dist is the distance from the listener to the source.\n
 * - REFERENCE_DISTANCE are the distance at which the listener will experience
 * GAIN. Both are set per source.\n
 * - ROLLOFF is a source specific factor of attenuation. If it's set to one,
 *   the InverseDistance model will describe a "physically correct" inverse
 *   square behaviour.\n
 * - MIN_GAIN, MAX_GAIN and MAX_DISTANCE are values used for clamping gain
 *   and distance, respectively.
 */
typedef enum DistanceModel {None,InverseDistance,InverseDistanceClamped};

/**
 * Class for setting global parameters.
 * This doesn't have to be instantiated if one does not need to set global
 * parameters.
 */
class AudioEnvironment : public AudioBase {
public:
  /**
   * Constructor.
   */
  AudioEnvironment() throw (InitError);
    
  /**
   * Constructor.
   * The parameters are ignored if this isn't the first object to be
   * instatiated of the AudioBase descendants.
   * @param frequency is the playing frequency of the enviroment (in Hz)
   * @param refresh is the refresh rate of the enviroment (in Hz)
   * @param synchronous is true if the enviroment is synchronous
   */
  AudioEnvironment(int frequency,int refresh,bool synchronous)
    throw (InitError);


  /**
   * Constructor.
   * The parameters are ignored if this isn't the first object to be
   * instatiated of the AudioBase descendants.
   * @param frequency is the playing frequency of the enviroment (in Hz)
   * @param refresh is the refresh rate of the enviroment (in Hz)
   */
  AudioEnvironment(int frequency,int refresh=-1)
    throw (InitError);

  /**
   * Sets the speed of sound in the enviroment.
   * This is used in doppler calculations.
   * @param speed is the speed of sound in length units per second.
   */
  void SetSoundSpeed(float speed) throw (ValueError,FatalError);

  /**
   * Get the speed of sound in the enviroment.
   * @return speed of sound in length units per second.
   */
  float GetSoundSpeed() throw (FatalError);

  /**
   * Sets the doppler factor.
   * This can be used to exaggerate, deemphasize or completely turn off the
   * doppler effect.
   * @param factor has a default value of one.
   */
  void SetDopplerFactor(float factor) throw (ValueError,FatalError);

  /**
   * Gets the doppler factor.
   * @return doppler factor.
   */
  float GetDopplerFactor() throw (FatalError);

  /**
   * Sets global gain (volume).
   * The volume a source will be played at will be multiplied by this _after_
   * the attenuation calculations.
   * Note: In todays's implementation on Linux, gain is clamped to [0.0,1.0].
   * This will be changed in future releases of OpenAL.
   * @param gain is the gain [0.0,...
   */
  void SetGain(float gain);
    
  /**
   * Gets the global gain
   * @return global gain
   */
  float GetGain() throw (FatalError);

  /**
   * Sets the distance model used in attenuation calculations.
   * @param model is one of: None, InverseDistance, InverseDistanceClamped.
   */
  void SetDistanceModel(DistanceModel model) throw (FatalError);

  /**
   * Gets the distance model used in attenuation calculations.
   * @return the model.
   */
  DistanceModel GetDistanceModel() throw (FatalError);
    
  /**
   * Initiates Loki's reverb implementation.
   */
  void InitiateReverb() throw (InitError);
};

}

#endif /* AUDIOENVIROMENT_H_INCLUDED_C4211030 */





