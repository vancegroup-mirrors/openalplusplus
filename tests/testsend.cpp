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
 *
 * Example where a soundfile is loaded using alutLoadWav and streamed over a UDP socket
 * to a receiver that will read the datastream and play it.
 */

#include <cc++/thread.h>
#include <cc++/socket.h>
#include <iostream>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alut.h>
#include "openalpp/windowsstuff.h"

using namespace ost;

int main(int argc,char **argv) {
  ALvoid *data;
  ALsizei bits,freq,size;
  ALenum format;
  ALboolean success;
  unsigned int packetsize=10000;       // Default packet size = 10000 bytes
  const char defaultfile[]="bee.wav";
  const char *filename;

  if(argc>1)
    filename=argv[1];
  else
    filename=defaultfile;
  if(argc>2)
    packetsize=atoi(argv[2]);

  try {
    success=alutLoadWAV(filename,&data,&format,&size,&bits,&freq);
    std::cerr << "Bits:  " << bits << " Freq: " << freq << std::endl;

    if(success==AL_FALSE) {
      std::cerr << "Error loading " << filename << "\n";
      exit(1);
    }

    UDPSocket socket;
    socket.setPeer(InetHostAddress("127.0.0.1"),33333);

    int totalsent=0;
    while(totalsent<size) {        // Send data in packets with a 100 ms delay between packets
      if((totalsent+packetsize)>size)
	      packetsize=size-totalsent;
      totalsent+=socket.send((char *)data+totalsent,packetsize);
      ost::Thread::sleep(100);
    }

    free(data);
  } catch(...) {
    std::cerr << "Error caught!\n";
  }

  return 0;
}
