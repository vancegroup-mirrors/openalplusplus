#include "netstream.h"

namespace openalpp {

NetStream::NetStream(ost::UDPSocket *socket,ost::TCPStream *controlsocket) 
  : Stream() {
  // TODO: Implement this
  if(controlsocket)
    ; // read stuff from it
  else
    ; // set defaults
  /*  updater_=new NetUpdater(socket_,controlsocket_,
			  buffername_,buffer2_->GetName(),
			  format_,frequency_,packetsize_);*/
}

NetStream::NetStream(ost::UDPSocket *socket,SampleFormat format,
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
  // TODO: Copy/Reference etc. updater..
}

NetStream::~NetStream() {
  delete updater_;
}

NetStream &NetStream::operator=(const NetStream &stream) {
  if(&stream!=this) {
    Stream::operator=((const Stream &)stream);
    // TODO: Delete/Dereference updater..
    // TODO: Copy/Reference etc. updater..
  }
  return *this;
}

}

