#ifndef SOURCE_H_INCLUDED_C419B739
#define SOURCE_H_INCLUDED_C419B739

#include "stream.h"
#include "sourcebase.h"
#include "sample.h"

namespace openalpp {

/**
 * Class for "normal" sources.
 * This is used for standard OpenAL sources, whether streaming or not.
 */
class Source : public SourceBase {
 public:
  /**
   * Constructor.
   * Creates the source with the specified position.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  Source(float x = 0.0, float y = 0.0, float z = 0.0);
  
  /**
   * Constructor.
   * Creates the source and a buffer with the specified file.
   * @param filename is the name of the file.
   */
  Source(const char *filename,float x=0.0,float y=0.0,float z=0.0);

  /**
   * Constructor.
   * Creates the source with the specified buffer.
   * @param buffer is the buffer to use.
   */
  Source(const Sample &buffer,float x=0.0,float y=0.0,float z=0.0);

  /**
   * Constructor.
   * Creates the source with the specified stream.
   * @param stream is the stream to use.
   */
  Source(const Stream &stream,float x=0.0,float y=0.0,float z=0.0);

  /**
   * Copy constructor.
   */
  Source(const Source &source);
    
  /**
   * Destructor.
   */
  ~Source();

  /**
   * Create a buffer for the source and load a file into it.
   * The source should _not_ be playing when doing this.
   * @param filename is the name of the file.
   */
  void SetSound(const char *filename);

  /**
   * Sets a new buffer for the source.
   * The source should _not_ be playing when doing this.
   * @param buffer is the new buffer.
   */
  void SetSound(const Sample &buffer);

  /**
   * Sets a new (streamed) buffer for the source.
   * The source should _not_ be playing when doing this.
   * @param stream is the new buffer.
   */
  void SetSound(const Stream &stream);

  /**
   * Gets the buffer associated with the source.
   * @return the buffer.
   */
  const SoundData &GetSound() const;

  /**
   * Play a file on the source.
   * This will change the source's buffer.
   * @param filename is the name of the file to play.
   */
  void Play(const char *filename);

  /**
   * Play a buffer on the source.
   * This will change the source's buffer.
   * @param buffer is the buffer to play.
   */
  void Play(const Sample &buffer);
    
  /**
   * Play a stream on the source.
   * This will change the source's buffer.
   * @param stream is the stream to play.
   */
  void Play(const Stream &stream);
    
  /**
   * Play this source.
   * This is only here, because the above Play(...) hides SourceBase::Play()
   */
  void Play();

  /**
   * Stop this source.
   * This is needed here for streaming sources...
   */
  void Stop();

  /**
   * Check if the source is streaming.
   * @return true if the source is streaming, false otherwise.
   */
  bool IsStreaming();
    
  /**
   * Assignment operator.
   */
  Source &operator=(const Source &source);

 private:
  /**
   * Flag for whether the source is streaming.
   */
  bool streaming_;

  /**
   * Pointer to the SoundData (buffer) associated with this source.
   */
  SoundData *sounddata_;
};

}

#endif /* SOURCE_H_INCLUDED_C419B739 */
