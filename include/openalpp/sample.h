/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Umeå University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *              CommonC++ (http://cplusplus.sourceforge.net/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#ifndef SAMPLE_H_INCLUDED_C4211F97
#define SAMPLE_H_INCLUDED_C4211F97

#include "openalpp/windowsstuff.h"
#include "openalpp/sounddata.h"
#include "openalpp/error.h"
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

/**
 * Check how large a sample is in the given format.
 */
unsigned int SampleSize(SampleFormat format);
unsigned int SampleSize(ALenum format);
}

#endif /* SAMPLE_H_INCLUDED_C4211F97 */
