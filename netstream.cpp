#include "netstream.h"

namespace openalpp {

NetStream::NetStream(ost::UDPSocket *socket,ost::TCPStream *controlsocket) 
  : Stream() {
  unsigned int frequency=11025,packetsize=1024;
  ALenum format=AL_FORMAT_MONO8;
  if(controlsocket) {
    *controlsocket >> format;
    *controlsocket >> frequency;
    *controlsocket >> packetsize;
  }
  updater_=new NetUpdater(socket,controlsocket,
			  buffername_,buffer2_->GetName(),
			  format,frequency,packetsize);
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
}

NetStream::~NetStream() {
}

NetStream &NetStream::operator=(const NetStream &stream) {
  if(&stream!=this) {
    Stream::operator=((const Stream &)stream);
  }
  return *this;
}

}

