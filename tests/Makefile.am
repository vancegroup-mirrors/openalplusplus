
INCLUDES = -I$(top_builddir)/include

if WITH_CCGNU
    ccgnulibs = @CCGNULIBS@
endif

if WITH_PORTAUDIO
    portaudiolibs = -lportaudio
endif

LDADD = ../src/libopenalpp.la $(ccgnulibs) -lopenal $(portaudiolibs)

if WITH_PORTAUDIO
    portaudiotests = testmic
endif

bin_PROGRAMS = teststream testsend $(portaudiotests) simple

teststream_SOURCES = teststream.cpp

testsend_SOURCES = testsend.cpp

testmic_SOURCES = testmic.cpp

simple_SOURCES = simple.cpp
