#ifndef NETUPDATER_H_INCLUDED_C419EF97
#define NETUPDATER_H_INCLUDED_C419EF97

#include "streamupdater.h"

namespace openalpp {

//##ModelId=3BDD3B340160
class NetUpdater : public StreamUpdater
{
  public:
    //##ModelId=3BDD3B46004E
    NetUpdater(/*SOCKET*/ int socket, ALuint buffer);

};

}

#endif /* NETUPDATER_H_INCLUDED_C419EF97 */
