TEMPLATE = app
CONFIG += core gui

# application name
TARGET = ledstripstudio

# let everything find the source directory without specifying its location
INCLUDEPATH += ./src/
DEPENDPATH = $$INCLUDEPATH

CONFIG += silent

SOURCES += \
    src/audioinput/audioinput.cpp \
    src/audioinput/audioinputobserver.cpp \
    src/audioinput/userdata.cpp \
    src/basic/animation.cpp \
    src/basic/color.cpp \
    src/basic/frame.cpp \
    src/basic/led.cpp \
    src/gui/ffttoolbar.cpp \
    src/gui/mainwindow.cpp \
    src/gui/ledstripemulator.cpp \
    src/gui/spectrumsettingswidget.cpp \
    src/gui/tonetoolbar.cpp \
    src/player/ledcontroller.cpp \
    src/player/player.cpp \
    src/settings/controlsettings.cpp \
    src/settings/configurationgroup.cpp \
    src/spectrum/fftw++.cpp \
    src/spectrum/spectrumanalyser.cpp \
    src/spectrum/spectrumobserver.cpp \
    src/spectrum/toneobserver.cpp \
    src/spectrum/toneanalyser.cpp \
    src/studio/imagestudio.cpp \
    src/studio/spectrumstudio.cpp \
    src/studio/studio.cpp \
    src/studio/tonestudio.cpp \
    src/main.cpp \
    src/gui/playersettingswidget.cpp \
    src/gui/ledstripstatuswidget.cpp \
    src/gui/actionconsistency.cpp

HEADERS += \
    src/audioinput/audioinput.h \
    src/audioinput/audioinputobserver.h \
    src/audioinput/userdata.h \
    src/basic/animation.h \
    src/basic/color.h \
    src/basic/frame.h \
    src/basic/led.h \
    src/basic/universalsleep.h \
    src/gui/ffttoolbar.h \
    src/gui/mainwindow.h \
    src/gui/ledstripemulator.h \
    src/gui/spectrumsettingswidget.h \
    src/gui/tonetoolbar.h \
    src/player/ledcontroller.h \
    src/player/player.h \
    src/settings/controlsettings.h \
    src/settings/configurationgroup.h \
    src/spectrum/spectrumanalyser.h \
    src/spectrum/spectrumobserver.h \
    src/spectrum/toneanalyser.h \
    src/spectrum/toneobserver.h \
    src/studio/studio.h \
    src/studio/spectrumstudio.h \
    src/studio/tonestudio.h \
    src/studio/imagestudio.h \
    src/gui/playersettingswidget.h \
    src/gui/ledstripstatuswidget.h \
    src/gui/actionconsistency.h

FORMS += \
    src/gui/mainwindow.ui \
    src/gui/spectrumsettingswidget.ui \
    src/gui/playersettingswidget.ui \
    src/gui/ledstripstatuswidget.ui

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
unix:INCLUDEPATH += ./include/
unix:INCLUDEPATH += ./include/fftwpp/

win32:INCLUDEPATH += C:/dev/ledstrip/include/
win32:INCLUDEPATH += C:/dev/ledstrip/include/fftwpp/
win32:INCLUDEPATH += C:/dev/tools/boost_1_50_0_build/include/

# library locations
unix:LIBS += -L./lib/ # location of FFTW and AudioPort libraries

win32:LIBS += -LC:/dev/ledstrip/lib/vs2010x86/ # location of FFTW and AudioPort libraries for windows
win32:LIBS += -LC:/dev/tools/boost_1_50_0_build/lib/

# libraries
unix:LIBS += -lportaudio -lasound -lfftw3 -lm
unix:LIBS += -lboost_thread -lboost_system

win32:LIBS += -lportaudio -llibfftw3-3

# compiler flags
unix:QMAKE_CXXFLAGS += -std=c++0x
unix:QMAKE_CXXFLAGS += -Wall
unix:QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
unix:QMAKE_LFLAGS += -Bstatic

# linker flags
QMAKE_LFLAGS += -time # show link time

# location of generated files
UI_DIR = ./src/gui/

CONFIG(debug, debug|release) {
  MAKEFILE = Makefile.debug
  OBJECTS_DIR = ./tmp/debug
  MOC_DIR = ./tmp/debug
} else {
  MAKEFILE = Makefile.release
  OBJECTS_DIR = ./tmp/release
  MOC_DIR = ./tmp/release
}
