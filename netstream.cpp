#include "netstream.h"

namespace openalpp {

NetStream::NetStream(UDPSocket *socket,TCPStream *controlsocket) 
  : Stream() {
  if(controlsocket)
    ; // read stuff from it
  else
    ; // set defaults
  /*  updater_=new NetUpdater(socket_,controlsocket_,
			  buffername_,buffer2_->GetName(),
			  format_,frequency_,packetsize_);*/
}

NetStream::NetStream(UDPSocket *socket,SampleFormat format,
		     unsigned int frequency,unsigned int packetsize) 
  : Stream() {
  ALenum alformat=0;
  switch(format) {
    case(Mono8):
      alformat=AL_FORMAT_MONO8;
      break;
    case(Mono16):
      alformat=AL_FORMAT_MONO16;
      break;
    case(Stereo8):
      alformat=AL_FORMAT_STEREO8;
      break;
    case(Stereo16):
      alformat=AL_FORMAT_STEREO16;
      break;
  }
  updater_=new NetUpdater(socket,NULL,buffername_,buffer2_->GetName(),
			  alformat,frequency,packetsize);
}


NetStream::NetStream(const NetStream &stream) : Stream((const Stream &)stream){
}

NetStream::~NetStream() {
}

NetStream &NetStream::operator=(const NetStream &stream) {
  return *this;
}

}

