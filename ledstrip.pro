TEMPLATE = app
CONFIG += core gui

# let everything find the source directory without specifying its location
INCLUDEPATH += ./src/
DEPENDPATH = $$INCLUDEPATH

SOURCES += \
    audioinput/audioinput.cpp \
    audioinput/audioinputobserver.cpp \
    audioinput/userdata.cpp \
    basic/animation.cpp \
    basic/color.cpp \
    basic/frame.cpp \
    basic/led.cpp \
    gui/mainwindow.cpp \
    gui/ledstripemulator.cpp \
    gui/spectrumsettingswidget.cpp \
    player/ledcontroller.cpp \
    player/player.cpp \
    settings/controlsettings.cpp \
    spectrum/fftw++.cpp \
    spectrum/spectrumanalyser.cpp \
    spectrum/spectrumobserver.cpp \
    spectrum/toneobserver.cpp \
    spectrum/toneanalyser.cpp \
    studio/imagestudio.cpp \
    studio/spectrumstudio.cpp \
    studio/studio.cpp \
    studio/tonestudio.cpp \
    main.cpp \
    src/gui/tonetoolbar.cpp

HEADERS += \
    audioinput/audioinput.h \
    audioinput/audioinputobserver.h \
    audioinput/userdata.h \
    basic/animation.h \
    basic/color.h \
    basic/frame.h \
    basic/led.h \
    basic/universalsleep.h \
    gui/mainwindow.h \
    gui/ledstripemulator.h \
    gui/spectrumsettingswidget.h \
    player/ledcontroller.h \
    player/player.h \
    settings/controlsettings.h \
    spectrum/spectrumanalyser.h \
    spectrum/spectrumobserver.h \
    spectrum/toneanalyser.h \
    spectrum/toneobserver.h \
    studio/studio.h \
    studio/spectrumstudio.h \
    studio/tonestudio.h \
    studio/imagestudio.h \
    src/gui/tonetoolbar.h

FORMS += \
    gui/mainwindow.ui \
    gui/spectrumsettingswidget.ui

OTHER_FILES += \
    icons/audio-volume-high.png \
    icons/wave_high_frequency.png \
    icons/color_wheel.png \
    icons/audio-x-generic.png \
    icons/audio-volume-muted.png \
    icons/rainbow-animation.png \
    icons/animation-mode.png \
    icons/smooth-tone-setting.png \
    icons/step-tone-setting.png \
    icons/color_wheel2.png \
    icons/preferences-system.png

# libraries
# 3 libraries are used:
# FFTW for the fast fourier transform
# AudioPort for audio input
# Boost for serial connection with the Arduino

# library header file directories
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
