#include <cc++/socket.h>
#include <cc++/thread.h>
#include <alpp/alpp.h>

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
