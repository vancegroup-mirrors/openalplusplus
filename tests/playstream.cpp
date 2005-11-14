/**
* OpenAL++ - an object oriented toolkit for spatial sound
* Copyright (C) 2002 VRlab, Umeå University
*
* OpenAL++ was created using the libraries:
*                 OpenAL (http://www.openal.org), 
*              PortAudio (http://www.portaudio.com/), and
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
*
* Example of streaming from a file. Only ogg files are supported at the
*/

#include <openalpp/alpp.h>
#include <iostream>


#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#ifdef WIN32
#include <crtdbg.h>
#endif



using namespace openalpp;


int main(int argc,char **argv) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <ogg-file>" << std::endl;
    return 0;
  }

  try 
    {

        openalpp::ref_ptr<Source> source = new Source;

        source->setSound(new FileStream(argv[1]));
        source->setAmbient();
        source->play();

        std::cout << "Press return to exit" << std::endl;
        std::cin.get();
    } 
    catch(openalpp::Error e) 
    {
        std::cerr << e << "\n";
    } 
    catch(...) 
    {
        std::cerr << "Unknown error!\n";
    }


    return 0;
}
