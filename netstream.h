#ifndef NETSTREAM_H_INCLUDED_C419F72E
#define NETSTREAM_H_INCLUDED_C419F72E

#include "netupdater.h"
#include "stream.h"

namespace openalpp {

//##ModelId=3BD87B580197
class NetStream : public Stream {
  //##ModelId=3BDEC206031E
  NetUpdater *updater_;
 public:
  //##ModelId=3BD87B7402AF
  NetStream(/*SOCKET*/int socket);

  //##ModelId=3BDD2D1D0069
  NetStream(const NetStream &stream);

  //##ModelId=3BE6462F030C
  ~NetStream();

  //##ModelId=3BDD2D420059
  NetStream &operator=(const NetStream &stream);
};

}

#endif /* NETSTREAM_H_INCLUDED_C419F72E */
