#include <cc++/socket.h>
#include <cc++/thread.h>
#include "../include/alpp.h"

using namespace openalpp;
using namespace ost;
using namespace std;

int main() {
  try {
    UDPSocket socket(InetAddress("127.0.0.1"),33333);
    NetStream stream(&socket,Mono8,11025,1024);
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
  ccxx_sleep(2000);
  return 0;
}
