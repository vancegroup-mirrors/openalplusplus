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
 */

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

  runmutex_.enterMutex();
  while(!stoprunning_) {
    runmutex_.leaveMutex();
    if(socket_->isPending(ost::Socket:pendingInput,1000)) {
      len=socket_->receive(buffer,packetsize_);
      Update(buffer,len);
    } else {
      if(controlsocket_ && 
	 controlsocket_->isPending(ost::Socket:pendingInput,100)) {
	char instr[100];
	*controlsocket_ >> instr;
	runmutex_.enterMutex();
	break;
      }
    }
    runmutex_.enterMutex();
  }
  runmutex_.leaveMutex();
  
  free(buffer);
}

}
