#include <cc++/thread.h>
#include <cc++/socket.h>
#include <iostream>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alut.h>
#include "windowsstuff.h"

using namespace ost;

int main(int argc,char **argv) {
  ALvoid *data;
  ALsizei bits,freq,size;
  ALenum format;
  ALboolean success;
  unsigned int packetsize=1024;

  try {
    alutInit(&argc,argv);

    success=alutLoadWAV("gamebeginning.wav",&data,&format,&size,&bits,&freq);
    if(success==AL_FALSE) {
      std::cerr << "Error loading\n";
      exit(1);
    }

    UDPSocket socket;
    socket.setPeer(InetHostAddress("127.0.0.1"),33333);

    int totalsent=0;
    while(totalsent<size) {
      if((totalsent+packetsize)>size)
	packetsize=size-totalsent;
      totalsent+=socket.Send((char *)data+totalsent,packetsize);
    }

    free(data);
  } catch(...) {
    std::cerr << "Error caught!\n";
  }

  return 0;
}
