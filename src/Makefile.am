
INCLUDES = -I$(top_builddir)/include

basicfiles = \
    audioenvironment.cpp \
    listener.cpp \
    audiobase.cpp \
    sourcebase.cpp \
    source.cpp \
    sample.cpp \
    sounddata.cpp \
    audioconvert.cpp \
    error.cpp \
    groupsource.cpp
streamfiles = \
    netstream.cpp \
    streamupdater.cpp \
    netupdater.cpp \
    stream.cpp
devicefiles = \
    deviceupdater.cpp \
    inputdevice.cpp

if WITH_CCGNU
if WITH_PORTAUDIO
libopenalpp_la_SOURCES = $(basicfiles) $(streamfiles) $(devicefiles)
else
libopenalpp_la_SOURCES = $(basicfiles) $(streamfiles)
endif
else
libopenalpp_la_SOURCES = $(basicfiles)
endif

lib_LTLIBRARIES = libopenalpp.la


