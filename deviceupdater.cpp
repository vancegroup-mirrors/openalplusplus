#include "deviceupdater.h"

namespace openalpp {

//##ModelId=3BDD3AC20062
DeviceUpdater::DeviceUpdater(ALenum format,unsigned int frequency,
			     ALuint buffer1,ALuint buffer2) 
  : StreamUpdater(buffer1,buffer2,format,frequency) {
}

}
