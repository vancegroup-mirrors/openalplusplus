#ifndef POSITIONEDOBJECT_H_INCLUDED_C4278579
#define POSITIONEDOBJECT_H_INCLUDED_C4278579

#include "audiobase.h"

namespace openalpp {

/**
 * Virtual base class for positioned objects.
 * (I.e. listeners and sources).
 */
class PositionedObject : public AudioBase {
 public:
  /**
   * Set position.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  virtual void SetPosition(float x, float y, float z)=0;

  /**
   * Get position.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  virtual void GetPosition(float &x, float &y, float &z) const =0;

  /**
   * Set velocity.
   * @param vx x member of velocity vector.
   * @param vy y member of velocity vector.
   * @param vz z member of velocity vector.
   */
  virtual void SetVelocity(float vx, float vy, float vz)=0;

  /**
   * Get velocity.
   * @param vx x member of velocity vector.
   * @param vy y member of velocity vector.
   * @param vz z member of velocity vector.
   */
  virtual void GetVelocity(float &vx, float &vy, float &vz) const =0;
};

}

#endif /* POSITIONEDOBJECT_H_INCLUDED_C4278579 */
