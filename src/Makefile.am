
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

if WITH_CCGNU
    streamfiles = \
    netstream.cpp \
    streamupdater.cpp \
    netupdater.cpp \
    stream.cpp
if WITH_PORTAUDIO
	devicefiles = \
	deviceupdater.cpp \
	inputdevice.cpp
endif
endif

lib_LTLIBRARIES = libopenalpp.la

libopenalpp_la_SOURCES = $(basicfiles) $(streamfiles) $(devicefiles)

