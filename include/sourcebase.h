/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Umeå University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *              CommonC++ (http://cplusplus.sourceforge.net/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#ifndef SOURCEBASE_H_INCLUDED_C419E330
#define SOURCEBASE_H_INCLUDED_C419E330
#include "positionedobject.h"
#include "error.h"

namespace openalpp {

/**
 * Enum for the different states of a source.
 * When a source is created, it is in the Initial state. Rewind will return a
 * source to this state.
 * The other states should be obvious.
 */
typedef enum SourceState {Initial,Playing,Paused,Stopped};

/**
 * Base class for sources.
 * This class holds functions for playing, setting position etc. However, it
 * cannot be instantiated, instead a source of either type (GroupSource or
 * Source) should be created.
 */
class SourceBase : public PositionedObject {
 public:
  /**
   * Play the source.
   */
  void Play();

  /**
   * Pause the source.
   */
  void Pause();

  /**
   * Stop the source.
   */
  void Stop();

  /**
   * Rewind the source.
   */
  void Rewind();
    
  /**
   * Get the current state.
   * @return one of Initial,Playing,Paused,Stopped
   */
  SourceState GetState() const;

  /**
   * Turn on/off looping.
   * @param loop is true if the source should loop, false otherwise.
   */
  void SetLooping(bool loop = true);

  /**
   * Check whether the source is looping.
   * @return true if it's looping, false otherwise.
   */
  bool IsLooping() const;

  /**
   * Sets the direction of the source.
   * @param direction x x value of the direction vector.
   * @param direction y y value of the direction vector.
   * @param direction z z value of the direction vector.
   */
  void SetDirection(float directionx, float directiony, float directionz);

  /**
   * Gets the direction of the source.
   * @param direction x x value of the direction vector.
   * @param direction y y value of the direction vector.
   * @param direction z z value of the direction vector.
   */
  void GetDirection(float &directionx,float &directiony,float &directionz) const;

  /**
   * Makes the source omni-directional.
   * The same effect can be achieved by calling SetDirection(0,0,0)
   */
  void MakeOmniDirectional();

  /**
   * Sets the sound cone parameters for a directional sound source.
   * This function has no effect on omni-directional sources. Two cones,  with
   * the top at the source, and turned the same direction as the source,
   * are defined by this. Inside the inner cone (specified by innerangle),
   * sound will be played at full volume (attenuated by distance), and
   * outside the outer cone (specified by outerangle) sound will be played
   * at the volume specified by outergain. Between these areas, the sound
   * volume will be interpolated between normal gain and outergain.
   * @param innerangle specifies the inner cone.
   * @param outerangle specifies the outer cone.
   * @param outergain specifies the gain outside the outer cone.
   */
  void SetSoundCone(float innerangle,
		    float outerangle=360.0,float outergain=0.0);

  /**
   * Gets the sound cone parameters.
   * @param innerangle specifies the inner cone.
   * @param outerangle specifies the outer cone.
   * @param outergain specifies the gain outside the outer cone.
   */
  void GetSoundCone(float &innerangle,float &outerangle,float &outergain)const;

  /**
   * Sets gain (volume).
   * The volume a source will be played at will be multiplied by this _after_
   * the attenuation calculations.
   * Note: In todays's implementation on Linux, gain is clamped to [0.0,1.0].
   * This will be changed in future releases of OpenAL.
   * @param gain is the gain [0.0,...
   */
  void SetGain(float gain);

  /**
   * Gets the gain (volume).
   * @return gain.
   */
  float GetGain() const;

  /**
   * Sets maximum and minimum gain this source will be played at.
   * I.e. the gain will be clamped to these values.
   * @param min is minimum gain.
   * @param max is maximum gain.
   */
  void SetMinMaxGain(float min = 0.0, float max = 1.0);

  /**
   * Gets maximum and minumum gain.
   * @param min is minimum gain.
   * @param max is maximum gain.
   */
  void GetMinMaxGain(float & min, float &max) const;

  /**
   * Makes the source ambient (or makes it stop being ambient).
   * This function _will_ change the source's position, direction and roll-off
   * factor.
   * @param ambient is true if the source should be ambient, false otherwise.
   */
  void SetAmbient(bool ambient = true);

  /**
   * Check if the source is ambient.
   * @return true if the source is ambient, false otherwise.
   */
  bool IsAmbient() const;

  /**
   * Makes the source's position relative (or makes it stop being relative).
   * This means that the position in space will not be absolute, but rather
   * relative to the listener.
   * @param relative is true if the source should be relative, false otherwise.
   */
  void SetRelative(bool relative = true);

  /**
   * Check if the source's position is relative.
   * @return true if the source is relative, false otherwise.
   */
  bool IsRelative() const;

  /**
   * Sets the reference distance for this source.
   * The reference distance is used in attenuation calculations.
   * @param distance is the reference distance.
   */
  void SetReferenceDistance(float distance = 1.0);

  /**
   * Gets the reference distance.
   * @return reference distance.
   */  
  float GetReferenceDistance() const;

  /**
   * Sets the maximum distance.
   * This is used in attenuation calculations, if the distance model is
   * InverseDistanceClamped.
   * @param distance is the maximum distance.
   */
  void SetMaxDistance(float distance); // = MAX_FLOAT);

  /**
   * Gets the maximum distance.
   * @return maximum distance.
   */
  float GetMaxDistance() const;

  /**
   * Sets the roll-off factor.
   * This is used in distance attenuation calculations.
   * @param factor is the rolloff factor.
   */
  void SetRolloffFactor(float factor = 1.0);

  /**
   * Gets the roll-off factor.
   * @return rolloff factor.
   */
  float GetRolloffFactor() const;

  /**
   * Sets the pitch.
   * 1.0 is normal. Each reduction by 50% equals a reduction by one octave.
   * @param pitch is the pitch (0.0,1.0].
   */
  void SetPitch(float pitch = 1.0);

  /**
   * Gets the pitch.
   * @return pitch.
   */
  float GetPitch() const;
    
  /** 
   * Set reverb scale for this source.
   * This is simply the scale of the "echo."
   * AudioEnviroment::InitiateReverb() must be called before this.
   * @param scale is the reverb scale [0.0-1.0].
   */
  void SetReverbScale(float scale) throw (InitError,ValueError);

  /**
   * Set reverb delay for this source.
   * AudioEnviroment::InitiateReverb() must be called before using this.
   * This is how many seconds back in time the echo will be.
   * @param delay is the delay [0.0-2.0] in seconds.
   */
  void SetReverbDelay(float delay) throw (InitError,ValueError);

  /**
   * Get reverb delay for this source.
   * @return the delay.
   */
  float GetReverbDelay() throw (InitError);

  /**
   * Get reverb scale for this source.
   * @return the scale.
   */
  float GetReverbScale() throw (InitError);

  /**
   * Link this source to another.
   * This causes calls to Play(), Pause(), Stop() and Rewind() (on this source)
   * to be applied to all sources this has been linked to, synchronously.
   * @param source is the source to link to.
   * @return identifier for the linked source. This is also the OpenAL name for
   * it.
   */
  ALuint Link(const SourceBase &source) throw (MemoryError);

  /**
   * Unlink this source from another.
   * @param source is the source to unlink.
   */
  void Unlink(const SourceBase &source) throw (NameError);

  /**
   * Unlink this source from another.
   * @param name is the name of the source to unlink.
   */
  void Unlink(const ALuint name) throw (NameError);

  /**
   * Unlink all sources from this.
   */
  void UnlinkAll();

  /**
   * Returns the OpenAL name of the source.
   * Can be used to directly modify the source with OpenAL functions.
   */
  ALuint GetAlSource() const;

  /**
   * Inherited from PositionedObject.
   */
  void SetPosition(float x, float y, float z);

  /**
   * Inherited from PositionedObject.
   */
  void GetPosition(float &x, float &y, float &z) const;    

  /**
   * Inherited from PositionedObject.
   */
  void SetVelocity(float vx, float vy, float vz);

  /**
   * Inherited from PositionedObject.
   */
  void GetVelocity(float &vx, float &vy, float &vz) const;
  
  /**
   * Assignment operator.
   */
  SourceBase &operator=(const SourceBase &sourcebase);

  /**
   * Destructor.
   */
  ~SourceBase();
 protected:
  /**
   * Constructor.
   */
  SourceBase() throw (MemoryError,NameError);

  /**
   * Constructor.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  SourceBase(float x,float y,float z) throw (MemoryError,NameError);

  /**
   * Copy constructor.
   */
  SourceBase(const SourceBase &sourcebase);
  
  /**
   * OpenAL name for this source.
   */
  ALuint sourcename_;

 private:
  /**
   * Initiate source.
   * Used by constructors.
   */
  void Init() throw (MemoryError,NameError);
  
  /**
   * Pointer to array of linked sources.
   */
  ALuint *linkedsources_;

  /**
   * #linked sources.
   */
  unsigned int nlinkedsources_;

  /**
   * size of array for linked sources.
   */
  unsigned int alloclinkedsources_;

  /**
   * Reverb scale and delay.
   */
  float reverbscale_,reverbdelay_;
};

}

#endif /* SOURCEBASE_H_INCLUDED_C419E330 */
