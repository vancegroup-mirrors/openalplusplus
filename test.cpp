#include "alpp.h"
#include <iostream>
#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#include <winbase.h>
#define sleep(x) Sleep(x*1000)
#endif

using namespace std;
using openalpp::AudioEnviroment;
using openalpp::None;
using openalpp::InverseDistance;
using openalpp::InverseDistanceClamped;
using openalpp::Source;
using openalpp::GroupSource;
using openalpp::Listener;
using openalpp::SourceState;
using openalpp::Playing;
using openalpp::Sample;
using openalpp::Error;

int main() {
  try {
    AudioEnviroment test;
    test.InitiateReverb();
    cerr << "Distance model used is: ";
    switch(test.GetDistanceModel()) {
      case(None):
	cerr << "None\n";
	break;
      case(InverseDistance):
	cerr << "InverseDistance\n";
	break;
      case(InverseDistanceClamped):
	cerr << "InverseDistanceClamped\n";
	break;
      default:
	cerr << "N/A\n";
    }

    /**********************************************************************
     * This should work, according to documentation:                      *
     *   test.SetGain(2.0);                                               *
     * But it doesn't... gain is clamped to [0.0,1.0] for listener        *
     * The same goes for sources, the following should work, but doesn't: *
     *   source.SetGain(2.0);                                             *
     **********************************************************************/

    cerr << "Creating source\n";
    Source source("gamebeginning.wav");
    Source source2("interm.wav"),source3("gamebeginning.wav");
    cerr << "Creating sample\n";
    Sample sample("gamebeginning.wav");
    cerr << "Creating sample 2=sample\n";
    Sample sample2=sample;
    cerr << "sample=source.GetSound()\n";
    sample=(Sample &)source.GetSound();
    cerr << "sample3(sample)\n";
    Sample sample3(sample);
    cerr << "Source is " << source.GetAlSource() << "\n";
    cerr << "Buffer is " << source.GetSound().GetAlBuffer() << "\n";
    Listener listener(1,2,3),listener2;
    float x,y,z;
    listener.GetPosition(x,y,z);
    cerr << "(" << x << "," << y << "," << z << ")\n";
    listener2=listener;
    listener.SetPosition(10.0,5.0,2.0);
    listener2.Select();
    if(listener.IsSelected())
      cerr << "Listener 1 is selected\n";
    if(listener2.IsSelected())
      cerr << "Listener 2 is selected\n";
    listener2.GetPosition(x,y,z);
    cerr << "(" << x << "," << y << "," << z << ")\n";
    source.SetPosition(2,2,3);
    source.SetRolloffFactor(0.3);
    source2.SetPosition(2,2,3);
    source2.SetRolloffFactor(0.3);
    /* Best not to use reverb: OpenAL segfaults on some files.
       source.SetReverbDelay(1.0);
       source.SetReverbScale(0.35);
    */
    /*
      source2.SetAmbient();
      source.SetAmbient();
      source.SetPitch(0.2);
      source2.SetPitch(0.2);
      cerr << "Pitch: " << source.GetPitch() << "," 
      << source2.GetPitch() << "\n";
    */

    ALuint name=source.Link(source3);
    source.Unlink(name);
    source.Link(source2);

    SourceState state;

    cerr << "\nCreating group source...\n";
    GroupSource gsource;
    cerr << "Including sources to it...\n";
    gsource.IncludeSource(&source);
    gsource.IncludeSource(&source2);
    gsource.SetAmbient();
    cerr << "Mixing it...\n";
    gsource.MixSources();
    cerr << "And playing it..\n";
    gsource.Play();
    state=gsource.GetState();
    if(state!=Playing)
      cerr << "Group source is not playing!\n";    
    sleep(7);
    gsource.Stop();
    //source.SetReverbScale(1.0);
    cerr << "Mixing again...\n";
    gsource.MixSources();
    cerr << "And playing...\n";
    gsource.Play();
    sleep(7);
    gsource.Stop();
    sleep(1);

    cerr << "\nPlaying source\n";
    source.Play();
    sleep(6);
    state=source.GetState();
    if(state!=Playing)
      cerr << "Source is not playing!\n";
    if(!alIsSource(source.GetAlSource()))
      cerr << "Source is not a source!\n";
    if(!alIsBuffer(source.GetSound().GetAlBuffer()))
      cerr << "Buffer is not a buffer!\n";
    state=source2.GetState();
    if(state!=Playing)
      cerr << "Source 2 is not playing!\n";
    state=source3.GetState();
    if(state==Playing)
      cerr << "Source 3 is playing!\n";
  } catch(Error e) {
    cerr << e << "\n";
  }
  ALenum error=alGetError();
  if(error!=AL_FALSE)
    cerr << "AL error not caught by OpenAL++ !\n";
  return 0;
}
