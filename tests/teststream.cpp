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
 * A sound Source is created and connected to the UDP stream of data
 */
#include <cc++/socket.h>
#include <cc++/thread.h>
#include <alpp.h>

using namespace openalpp;
using namespace ost;
using namespace std;

int main(int argc,char **argv) {
  unsigned int packetsize=10000;
  if(argc>1)
    packetsize=atoi(argv[1]);
  
  try {
    // Create a socket and listen for streamed data
    UDPSocket socket(InetAddress("127.0.0.1"),33333);

    // Create a soundstream that will read its data from the socket
    NetStream stream(&socket,Mono16,44100,packetsize);

    // Create a soundsource that uses the net-streamed soundstream.
    Source source(stream);

    source.SetAmbient();
    source.Play();
    cerr << "Press return to continue\n";
    cin.get();
    source.Stop();
  } catch(openalpp::Error e) {
    cerr << e << "\n";
  } catch(...) {
    cerr << "Unknown error!\n";
  }
  return 0;
}
