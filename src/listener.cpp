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

#include "openalpp/listener.h"

namespace openalpp {

Listener *Listener::selectedlistener_=NULL;

/**
 * Internal function for initialisation. 
 */
void Listener::Init(float x,float y,float z,
		    float vx,float vy,float vz,
		    float directionx, float directiony, float directionz,
		    float upx, float upy, float upz) {
  SetPosition(x,y,z);
  SetVelocity(vx,vy,vz);
  SetOrientation(directionx,directiony,directionz,
		 upx,upy,upz);
  if(!selectedlistener_)
    Select();
}

Listener::Listener() : PositionedObject() {
  Init(0,0,0, 0,0,0, 0,0,-1, 0,1,0);
}

Listener::Listener(const Listener &listener) : PositionedObject() {
  listener.GetPosition(position_[0],position_[1],position_[2]);
  listener.GetVelocity(velocity_[0],velocity_[1],velocity_[2]);
  listener.GetOrientation(orientation_[0],orientation_[1],orientation_[2],
			  orientation_[3],orientation_[4],orientation_[5]);
}

Listener::Listener(float x,float y,float z,
		   float directionx, float directiony, float directionz,
		   float upx, float upy, float upz) : PositionedObject() {
  Init(x,y,z, 0,0,0, directionx,directiony,directionz, upx,upy,upz);
}

Listener::Listener(float x, float y, float z) : PositionedObject() {
  Init(x,y,z, 0,0,0, 0,0,-1, 0,1,0);
}

Listener::~Listener() {
  if(selectedlistener_==this)
    selectedlistener_=NULL;
}

void Listener::Select() {
  alListenerfv(AL_POSITION,position_);
  alListenerfv(AL_VELOCITY,velocity_);
  alListenerfv(AL_ORIENTATION,orientation_);
  selectedlistener_=this;
}

bool Listener::IsSelected() {
  return (selectedlistener_==this);
}

void Listener::SetOrientation(float directionx,float directiony,
			      float directionz,
			      float upx, float upy, float upz) {
  orientation_[0]=directionx;
  orientation_[1]=directiony;
  orientation_[2]=directionz;
  orientation_[3]=upx;
  orientation_[4]=upy;
  orientation_[5]=upz;
  if(IsSelected())
    alListenerfv(AL_ORIENTATION,orientation_);
}

void Listener::GetOrientation(float &directionx,float &directiony,
			      float &directionz,
			      float &upx, float &upy, float &upz) const {
  directionx=orientation_[0];
  directiony=orientation_[1];
  directionz=orientation_[2];
  upx=orientation_[3];
  upy=orientation_[4];
  upz=orientation_[5];
}

Listener &Listener::operator=(const Listener &listener) {
  if(this!=&listener) {
    listener.GetPosition(position_[0],position_[1],position_[2]);
    listener.GetVelocity(velocity_[0],velocity_[1],velocity_[2]);
    listener.GetOrientation(orientation_[0],orientation_[1],orientation_[2],
			    orientation_[3],orientation_[4],orientation_[5]);
  }
  return *this;
}

void Listener::SetPosition(float x, float y, float z) {
  position_[0]=x;
  position_[1]=y;
  position_[2]=z;
  if(IsSelected())
    alListenerfv(AL_POSITION,position_);
}

void Listener::GetPosition(float &x, float &y, float &z) const {
  x=position_[0];
  y=position_[1];
  z=position_[2];
}

void Listener::SetVelocity(float vx, float vy, float vz) {
  velocity_[0]=vx;
  velocity_[1]=vy;
  velocity_[2]=vz;
  if(IsSelected())
    alListenerfv(AL_VELOCITY,velocity_);
}

void Listener::GetVelocity(float &vx, float &vy, float &vz) const {
  vx=velocity_[0];
  vy=velocity_[1];
  vz=velocity_[2];
}

}
