/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Ume� University
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
 *  
 * Simple example for using OpenAL++.
 * Plays a file bee.wav and moves it left right. 
 */
#include <openalpp/alpp.h>
#include <iostream>


using namespace openalpp;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


int main() {
  try {
    openalpp::ref_ptr<Source> beesound = new Source("bee.wav");
    beesound->setGain(1);
    beesound->setLooping();

    float limits[2] = {5,-15};
    float delay=10;
    float time=0,angle=0;
    
    beesound->setPosition(limits[0],0.0,0.0);
    beesound->play();
    
    const int no_laps=5;
    
    std::cerr << "Moving sound 5 laps..." << std::endl;
    
    // Do a cheat time loop.
    while(angle<(M_PI*2.0*no_laps)) {
      usleep(delay*1000); // Wait for delay milliseconds

      time +=delay/1000; // Calculate the time in the loop
      angle=M_PI *time;  // What is the resulting angle
      
      // Calculate a new position
      beesound->setPosition(limits[0]*cos(angle),0.0,limits[1]*sin(angle));
    }
  } catch(Error e) {
    std::cerr << e << "\n";
    return -1;
  }
  return 0;
}
  
