/**
 * Simple example for using OpenAL++.
 * Plays a file bee.wav and moves it left right. 
 */
#include <alpp.h>
#include <cc++/thread.h>
#include <iostream>

using namespace openalpp;
using namespace std;
using namespace ost;


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


int main() {
  try {
    Source beesound("bee.wav");
	  beesound.SetGain(1);
	  beesound.SetLooping();

    float limits[2] = {0.5,-0.5};
    float delay=10;
    float time=0,angle=0;
    
    beesound.SetPosition(limits[0],0.0,0.0);
	  beesound.Play();


    // Do a cheat time loop.
    while(angle<(M_PI*60)) {
	    ccxx_sleep(delay); // Wait for delay milliseconds

      time +=delay/1000; // Calculate the time in the loop
      angle=M_PI *time;  // What is the resulting angle
      
      // Calculate a new position
      beesound.SetPosition(1.0*cos(angle),0.0,1.0*sin(angle));
	  }
  } catch(Error e) {
    cerr << e << "\n";
    return -1;
  }
  return 0;
}
  