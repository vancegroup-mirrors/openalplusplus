/**
 * Simple example for using OpenAL++.
 * Plays a file (named back.wav, not included) in the background.
 */
#include <alpp/alpp.h>
#include <cc++/thread.h>
#include <iostream>

using namespace openalpp;
using namespace std;
using namespace ost;

int main() {
  try {
    Source bgsound("back.wav");   // Create source and load sound
    bgsound.SetGain(0.6);	  // Lower gain (volume) 
    bgsound.SetAmbient();	  // Make sound ambient (i.e. not attenuated)
    bgsound.Play();		  // Start playing
    ccxx_sleep(10000);		  // Sleep for a while
  } catch(Error e) {
    cerr << e << "\n";
    return -1;
  }
  return 0;
}
