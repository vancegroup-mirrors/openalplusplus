/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Ume� University
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
 *
 * Simple example for using OpenAL++.
 * Plays a file bee.wav in the background.
 */
#include <alpp.h>
#include <cc++/thread.h>
#include <iostream>

using namespace openalpp;
using namespace std;
using namespace ost;

int main() {
  try {
    Source bgsound("bee.wav");   // Create source and load sound
    bgsound.SetGain(0.6);	  // Lower gain (volume) 
	  bgsound.SetLooping();
    bgsound.SetAmbient();	  // Make sound ambient (i.e. not attenuated)
    bgsound.Play();		  // Start playing
    
    
    std::cerr << "playing bee.wav for 3 seconds: " << std::endl;
    ost::Thread::sleep(3000);		  // Sleep for a while
  } catch(Error e) {
    cerr << e << "\n";
    return -1;
  }
  return 0;
}
