#ifndef NETSTREAM_H_INCLUDED_C419F72E
#define NETSTREAM_H_INCLUDED_C419F72E

#include "netupdater.h"
#include "stream.h"

namespace openalpp {

/**
 * Class for handling streams through sockets.
 * Preliminary tests indicate that packets smaller than ca 1 kb should not
 * be used (tests were done with Mono8, 11025 Hz). 
 */
class NetStream : public Stream {
 public:
  /**
   * Constructor.
   * @param socket is the socket to stream data through.
   * @param controlsocket is an (optional) TCPStream that can be used to send
   * information about the stream. The constructor will begin with trying to
   * read SampleFormat, frequency, and packetsize. The sender can also use the
   * control socket to send "EXIT" when it's run out of data to send. If
   * this parameter is not given, defaults will be used (format=Mono8,
   * frequency=11025, packetsize=1024).
   */
  NetStream(UDPSocket *socket,TCPStream *controlsocket=NULL);

  /**
   * Constructor.
   * @param socket is the socket to stream data through.
   * @param format is the format the data will be in.
   * @param frequency is the frequency of the sound.
   * @param packetsize is the size of the packets the sound will be sent in.
   */
  NetStream(UDPSocket *socket,SampleFormat format,unsigned int frequency,
	    unsigned int packetsize);

  /**
   * Copy constructor.
   */
  NetStream(const NetStream &stream);

  /**
   * Destructor.
   */
  ~NetStream();

  /**
   * Assignment operator.
   */
  NetStream &operator=(const NetStream &stream);
};

}

#endif /* NETSTREAM_H_INCLUDED_C419F72E */
