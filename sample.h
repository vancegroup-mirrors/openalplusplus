#ifndef SAMPLE_H_INCLUDED_C4211F97
#define SAMPLE_H_INCLUDED_C4211F97

#include "windowsstuff.h"
#include "sounddata.h"
#include "error.h"
extern "C" {
#include <AL/al.h>
#include <AL/alut.h>
}
#include <string>

namespace openalpp {

/**
 * Class for loading sampled files.
 */
class Sample : public SoundData {
 public:
  /**
   * Constructor.
   * @param filename is name of file to load.
   */
  Sample(const char *filename) throw (FileError);

  /**
   * Copy constructor.
   */
  Sample(const Sample &sample);

  /**
   * Get file name of loaded file.
   * @return file name.
   */
  std::string GetFileName() const;

  /**
   * Assignment operator.
   */
  Sample &operator=(const Sample &sample);

 private:
  /**
   * File name.
   */
  std::string filename_;

};

}

#endif /* SAMPLE_H_INCLUDED_C4211F97 */
