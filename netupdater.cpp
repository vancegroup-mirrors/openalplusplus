#include "netupdater.h"

namespace openalpp {

NetUpdater::NetUpdater(ost::UDPSocket *socket,ost::TCPStream *controlsocket,
		       const ALuint buffer1,ALuint buffer2,
		       ALenum format,unsigned int frequency,
		       unsigned int packetsize) 
  : StreamUpdater(buffer1,buffer2,format,frequency), socket_(socket)
  , controlsocket_(controlsocket), packetsize_(packetsize) {
}

void NetUpdater::Run() {
  void *buffer=malloc(packetsize_);
  unsigned int len;

  while(1) {
    if(socket_->isPending(ost::SOCKET_PENDING_INPUT,2000)) {
      len=socket_->Recv(buffer,packetsize_);
      Update(buffer,len);
    } else {
      Sleep(50);
      if(controlsocket_ && 
	 controlsocket_->isPending(ost::SOCKET_PENDING_INPUT,100)) {
	char instr[100];
	*controlsocket_ >> instr;
	break;
      }
    }
  }
  
  free(buffer);
}

}
