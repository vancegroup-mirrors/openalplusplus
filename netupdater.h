#ifndef NETUPDATER_H_INCLUDED_C419EF97
#define NETUPDATER_H_INCLUDED_C419EF97

#include <cc++/socket.h>
#include "streamupdater.h"
#include "stdlib.h"

namespace openalpp {

/**
 * Updater for streams through sockets.
 */
class NetUpdater : public StreamUpdater {
  /**
   * Socket to receive sound data from.
   */
  UDPSocket *socket_;

  /**
   * Stream to receive control messages from.
   */
  TCPStream *controlsocket_;

  /**
   * Size of sound packets (in bytes).
   */
  unsigned int packetsize_;
 public:
  /**
   * Constructor.
   * @param socket is the socket to receive sound data from.
   * @param controlsocket is the stream to receive control messages from. Set
   * to NULL if control messages are not used.
   * @param buffer1 and...
   * @param buffer2 are the sound buffers to use.
   * @param format is the (OpenAL) format of the sound data.
   * @param frequency is the frequency of the sound data.
   * @param packetsize is the size of the sound packets.
   */
  NetUpdater(UDPSocket *socket,TCPStream *controlsocket,
	     const ALuint buffer1,ALuint buffer2,
	     ALenum format,unsigned int frequency,
	     unsigned int packetsize);

  /**
   * Inherited from Thread.
   * This will be called when the updater is Start():ed..
   */
  void Run();
};

}

#endif /* NETUPDATER_H_INCLUDED_C419EF97 */
