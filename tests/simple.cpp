/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Umeå University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
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
#include <openalpp/alpp.h>
#include <iostream>
#include <unistd.h>

using namespace openalpp;


int main(int argc,char **argv) {

  std::string file;
  file = "bee.wav";

  if (argc > 1)
    file = argv[1];

  std::cout << "Playing " << file << "\n";

  try {
    Source bgsound(file);   // Create source and load sound
    bgsound.setGain(0.6f);	  // Lower gain (volume) 
    bgsound.setLooping();
    bgsound.setAmbient();	  // Make sound ambient (i.e. not attenuated)
    bgsound.play();		  // Start playing
    
    
    std::cerr << "playing " << file << " for 3 seconds: " << std::endl;
    usleep(3000*1000);		  // Sleep for a while
    std::cerr << "Done " << std::endl;
    bgsound.stop();

  } catch(Error e) {
    std::cerr << e << "\n";
    return -1;
  }
  return 0;
}
