/* g++ -Wall -O3 -L. -lopenal -lpthread -lccgnu -lportaudio -lopenalpp
 *     testmic.cpp -o testmic
 * LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH testmic
 */
#include "alpp.h"
#include <iostream>

using namespace openalpp;
using namespace ost;
using namespace std;

int main() {
  InputDevice mic;
  Source source(mic);
  source.Play();

  cerr << "Press return to continue\n";
  cin.get();
  return 0;
}
