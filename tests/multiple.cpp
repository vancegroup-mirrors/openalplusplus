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
 *  
 */
#include <openalpp/alpp.h>
#include <iostream>
#include <string>
#include <vector>

using namespace openalpp;

int main() 
{
  openalpp::ref_ptr<Sample> sample;

  std::cerr << "Loads a sample at the time, plays it for a while, then loads another one etc.." << std::endl;
  try {

    std::vector<std::string> file_vector;
    
    file_vector.push_back("a.wav");
    file_vector.push_back("high-e.wav");
    file_vector.push_back("low-e.wav");

    openalpp::ref_ptr<Source> source = new Source;
    unsigned int delay = 500;
    for(int i = 0; i < 10; i++) {
      if (sample.valid()) {
        source->stop();
      }
      std::string file = file_vector[i % file_vector.size()];
      std::cerr << "Loading and playing " << file << " for " << delay/1000.0 << " seconds" << std::endl;
      sample = new Sample(file);
      source->setSound(sample.get());     
      source->setGain(1);
      source->setLooping();
      source->play();
      usleep(delay*1000); // Wait for delay milliseconds
    }
  } catch(Error e) {
    std::cerr << e << "\n";
  }

  return 0;
}
  
