
INCLUDES = -I$(top_builddir)/include

if WITH_CCGNU
ccgnulibs = @CCGNULIBS@
endif
if WITH_PORTAUDIO
portaudiolibs = -lportaudio
endif
if WITH_OGGVORBIS
ogglibs = -lvorbisfile -lvorbis -logg
endif


LDADD = ../src/libopenalpp.la $(ccgnulibs) -lopenal $(portaudiolibs) $(ogglibs) -lOpenThreads

bin_PROGRAMS = testfstream multiple testmic simple moving #testsend teststream  

#teststream_SOURCES = teststream.cpp

#testsend_SOURCES = testsend.cpp

testmic_SOURCES = testmic.cpp

simple_SOURCES = simple.cpp

moving_SOURCES = moving.cpp


testfstream_SOURCES = testfstream.cpp

multiple_SOURCES = multiple.cpp
