#include <cc++/thread.h>
#include <cc++/socket.h>
#include <iostream>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alut.h>
#include "../include/windowsstuff.h"

using namespace ost;

int main(int argc,char **argv) {
  ALvoid *data;
  ALsizei bits,freq,size;
  ALenum format;
  ALboolean success;
  unsigned int packetsize=10000;       // Default packet size = 10000 bytes

  if(argc>1)
    packetsize=atoi(argv[1]);

  try {
    success=alutLoadWAV("bee.wav",&data,&format,&size,&bits,&freq);
    std::cerr << "Bits:  " << bits << " Freq: " << freq << std::endl;

    if(success==AL_FALSE) {
      std::cerr << "Error loading\n";
      exit(1);
    }

    UDPSocket socket;
    socket.setPeer(InetHostAddress("127.0.0.1"),33333);

    int totalsent=0;
    while(totalsent<size) {        // Send data in packets with a 100 ms delay between packets
      if((totalsent+packetsize)>size)
	      packetsize=size-totalsent;
      totalsent+=socket.Send((char *)data+totalsent,packetsize);
      ccxx_sleep(100);
    }

    free(data);
  } catch(...) {
    std::cerr << "Error caught!\n";
  }

  return 0;
}
