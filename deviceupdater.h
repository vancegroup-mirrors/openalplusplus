#ifndef DEVICEUPDATER_H_INCLUDED_C419EA9C
#define DEVICEUPDATER_H_INCLUDED_C419EA9C

#include "streamupdater.h"

namespace openalpp {

//##ModelId=3BDD3A4301B4
class DeviceUpdater : public StreamUpdater {
  public:
    //##ModelId=3BDD3AC20062
    DeviceUpdater(ALenum format, unsigned int frequency,
		  ALuint buffer1,ALuint buffer2);

};

}

#endif /* DEVICEUPDATER_H_INCLUDED_C419EA9C */
