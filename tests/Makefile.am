
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


noinst_PROGRAMS = testfstream multiple testmic simple moving 

testmic_SOURCES = testmic.cpp

simple_SOURCES = simple.cpp

moving_SOURCES = moving.cpp

testfstream_SOURCES = testfstream.cpp

multiple_SOURCES = multiple.cpp
