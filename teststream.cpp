#include <cc++/socket.h>
#include <cc++/thread.h>
#include "alpp.h"

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
    ccxx_sleep(10000);
  } catch(openalpp::Error e) {
    cerr << e << "\n";
  } catch(...) {
    cerr << "Unknown error!\n";
  }

  return 0;
}
