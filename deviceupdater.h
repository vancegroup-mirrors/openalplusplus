#ifndef DEVICEUPDATER_H_INCLUDED_C419EA9C
#define DEVICEUPDATER_H_INCLUDED_C419EA9C

#include "streamupdater.h"

namespace openalpp {

typedef enum SampleFormat {Mono8,Stereo8,Mono16,Stereo16};

//##ModelId=3BDD3A4301B4
class DeviceUpdater : public StreamUpdater {
  public:
    //##ModelId=3BDD3AC20062
    DeviceUpdater(SampleFormat format, unsigned int samplerate, ALuint buffer);

};

}

#endif /* DEVICEUPDATER_H_INCLUDED_C419EA9C */
