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

#include "openalpp/filestreamupdater.h"
#include "openalpp/filestream.h"
#include "openalpp/sample.h"

#ifdef _WIN32 /* We need the following two to set stdin/stdout to binary */
#include <io.h>
#include <fcntl.h>
#endif

namespace openalpp {

FileStream::FileStream(const char *filename,const int buffersize)
  throw (NameError,InitError,FileError) : Stream() {

  FILE *filehandle=fopen(filename,"r");
  if(!filehandle)
    throw FileError("FileStream: Couldn't open file");

#ifdef _WIN32 /* We need to set stdin/stdout to binary mode. Damn windows. */
  /* Beware the evil ifdef. We avoid these where we can, but this one we 
     cannot. Don't add any more, you'll probably go to hell if you do. */
  _setmode( _fileno( filehandle ), _O_BINARY );
  _setmode( _fileno( filehandle ), _O_BINARY );
#endif

  // Check for file type, create a FileStreamUpdater if a known type is
  // detected, otherwise throw an error.
  OggVorbis_File oggfile;
  if(ov_open(filehandle,&oggfile,NULL,0)>=0) {
    vorbis_info *ogginfo=ov_info(&oggfile,-1);
    ALenum format;
    if(ogginfo->channels==1) // We'll request 16 bit later...
      format=AL_FORMAT_MONO16;
    else
      format=AL_FORMAT_STEREO16;
    updater_=new FileStreamUpdater(oggfile,
				   buffername_,buffer2_->GetName(),
				   format,ogginfo->rate,
				   buffersize*SampleSize(format)); 
  } else {
    fclose(filehandle);
    throw FileError("FileStream: File of unknown type");
  }
}

FileStream::FileStream(const FileStream &stream)
  : Stream((const Stream &)stream) {
}
  
FileStream::~FileStream() {
}

FileStream &FileStream::operator=(const FileStream &stream) {
  if(&stream!=this) {
    Stream::operator=((const Stream &)stream);
  }
  return *this;
}

void FileStream::SetLooping(bool loop) {
  ((FileStreamUpdater *)updater_)->SetLooping(loop);
}

}
