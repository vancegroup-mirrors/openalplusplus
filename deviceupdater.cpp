#include "deviceupdater.h"

namespace openalpp {

//##ModelId=3BDD3AC20062
DeviceUpdater::DeviceUpdater(SampleFormat format,unsigned int samplerate,
			     ALuint buffer) : StreamUpdater(buffer) {
}

}
