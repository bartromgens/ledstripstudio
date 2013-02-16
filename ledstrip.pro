TEMPLATE = app
CONFIG += core gui

# let everything find the source directory without specifying its location
INCLUDEPATH += ./src/
DEPENDPATH = $$INCLUDEPATH

SOURCES += \
    audioinput/audioinput.cpp \
    audioinput/userdata.cpp \
    basic/animation.cpp \
    basic/color.cpp \
    basic/frame.cpp \
    basic/led.cpp \
    gui/mainwindow.cpp \
    gui/ledstripemulator.cpp \
    player/ledcontroller.cpp \
    player/player.cpp \
    settings/controlsettings.cpp \
    spectrum/fftw++.cpp \
    spectrum/spectrumanalyser.cpp \
    studio/studio.cpp \
    main.cpp \
    src/spectrum/toneanalyser.cpp \
    src/studio/spectrumstudio.cpp \
    src/audioinput/audioinputobserver.cpp \
    src/spectrum/spectrumobserver.cpp \
    src/spectrum/toneobserver.cpp

HEADERS += \
    audioinput/audioinput.h \
    audioinput/userdata.h \
    basic/animation.h \
    basic/color.h \
    basic/frame.h \
    basic/led.h \
    basic/universalsleep.h \
    gui/mainwindow.h \
    gui/ledstripemulator.h \
    player/ledcontroller.h \
    player/player.h \
    settings/controlsettings.h \
    spectrum/spectrumanalyser.h \
    studio/studio.h \
    src/spectrum/toneanalyser.h \
    src/studio/spectrumstudio.h \
    src/audioinput/audioinputobserver.h \
    src/spectrum/spectrumobserver.h \
    src/spectrum/toneobserver.h

FORMS += \
    gui/mainwindow.ui

OTHER_FILES += \
    icons/audio-volume-high.png \
    icons/wave_high_frequency.png \
    icons/color_wheel.png \
    icons/audio-volume-muted.svg \
    icons/audio-volume-high.svg \
    icons/audio-x-generic.svg \
    icons/audio-x-generic.png \
    icons/audio-volume-muted.png

#library header file directories
unix:INCLUDEPATH += /home/bart/prog/ledstrip/include/
unix:INCLUDEPATH += /home/bart/prog/ledstrip/include/fftwpp/
unix:INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0/boost/

win32:INCLUDEPATH += C:/prog/ledstrip/include/
win32:INCLUDEPATH += C:/prog/ledstrip/include/fftwpp/
win32:INCLUDEPATH += C:/prog/tools/boost_1_50_0_build/include/

# library locations
unix:LIBS += -L/home/bart/prog/ledstrip/lib/ # location of FFTW and AudioPort libraries
unix:LIBS += -L/home/bart/prog/1_tools/boost_1_52_0/stage/ # location of boost ./lib/ directory

win32:LIBS += -LC:/prog/ledstrip/lib/vs2010x86/ # location of FFTW and AudioPort libraries for windows
win32:LIBS += -LC:/prog/tools/boost_1_50_0_build/lib/

# libraries
unix:LIBS += -lportaudio -lasound -lfftw3 -lm
unix:LIBS += -lboost_thread -lboost_system

win32:LIBS += -lportaudio -llibfftw3-3

# compiler flags
unix:QMAKE_CXXFLAGS += -std=c++0x
unix:QMAKE_CXXFLAGS += -Wall

# linker flags
QMAKE_LFLAGS += -time # show link time

# location of generated files
OBJECTS_DIR = ./tmp/
MOC_DIR = ./tmp/
UI_DIR = ./src/gui/
