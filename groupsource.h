#ifndef GROUPSOURCE_H_INCLUDED_C427B440
#define GROUPSOURCE_H_INCLUDED_C427B440

#include "sourcebase.h"
#include "source.h"
//#ifndef WIN32
//extern "C" {
//#include "AL/alexttypes.h"
//}
//#endif
#include <vector>
#include <math.h>

namespace openalpp {

/**
 * Class for group sources.
 * Used for mixing together several sources _before_ they are played. This can
 * be used to play more sounds with less processing power. Of course the
 * problem is that the sources cannot be moved separately.
 */
class GroupSource : public SourceBase {
  /**
   * Flag for whether the sources have been mixed yet.
   */
  bool mixed_;

  /**
   * Vector containing the sources in the group.
   */
  std::vector<Source *> sources_;

  /**
   * OpenAL buffer for the group source.
   */
  ALuint buffer_;

  typedef enum Speaker {Left,Right};

  /**
   * Handles distance attenuation and directional sources.
   * @param source is the source to filter.
   * @return the gain.
   */
  ALfloat GroupSource::FilterDistance(ALuint source,Speaker speaker);

  /**
   * Reverb filter.
   * @param source is (a pointer to) the source.
   * @param buffer is (a pointer to) the buffer (=memory area).
   * @param size is the size of the buffer.
   */
  void GroupSource::FilterReverb(Source *source,ALshort *buffer,ALsizei size);

  /**
   * Apply filters.
   * @param source is (a pointer to) the source.
   * @param buffer is the buffer containing the sound.
   * @param size is the size of the buffer.
   */
  void ApplyFilters(Source *source,ALshort *buffer,ALsizei size);
 public:
  /**
   * Constructor.
   * Creates the group source at the specified position.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  GroupSource(float x = 0.0, float y = 0.0, float z = 0.0) throw (NameError);

  /**
   * Same as SourceBase::Play, except that this mixes the sources in the
   * group if it haven't been done yet.
   */
  void Play() throw (InitError,FileError);

  /**
   * Mix all added sources into one. This function will be called by
   * Play(), if sources have been included since the last time MixSamples()
   * was called, so if you want the source to start playing as fast as
   * possible after the Play()-call, MixSources() should be called
   * separately
   */
  void MixSources() throw (InitError,FileError,FatalError);

  /**
   * Includes a source in the group.
   * Returns an identifier that can be used as an argument to ExcludeSource(). 
   * This identifier is also the OpenAL name for the included source.
   * @param source is (a pointer to) the source to include.
   * @return identifier for the source.
   */
  ALuint IncludeSource(Source *source) throw (ValueError);

  /**
   * Removes a source from the group.
   * This will of course require the remaining sources to be mixed again.
   * @param source is the source to exclude.
   */
  void ExcludeSource(const Source &source);

  /**
   * Removes a source from the group.
   * This will of course require the remaining sources to be mixed again.
   * @param source is the identifier of the source to exclude.
   */
  void ExcludeSource(ALuint source);

  /**
   * Copy constructor.
   */
  GroupSource(const GroupSource &groupsource);

  /**
   * Destructor.
   */
  ~GroupSource();

  /**
   * Assignment operator.
   */
  GroupSource &operator=(const GroupSource &groupsource);
};

}

#endif /* GROUPSOURCE_H_INCLUDED_C427B440 */
