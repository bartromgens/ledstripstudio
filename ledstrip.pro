TEMPLATE = app
CONFIG += core gui

SOURCES += \
    src/gui/mainwindow.cpp \
    src/audioinput/audioinput.cpp \
    src/audioinput/userdata.cpp \
    src/basic/animation.cpp \
    src/basic/color.cpp \
    src/basic/frame.cpp \
    src/basic/led.cpp \
    src/spectrum/fftw++.cpp \
    src/spectrum/spectrumanalyser.cpp \
    src/player/ledcontroller.cpp \
    src/player/player.cpp \
    src/settings/controlsettings.cpp \
    src/studio/studio.cpp \
    src/main.cpp \

HEADERS += \
    src/audioinput/audioinput.h \
    src/audioinput/userdata.h \
    src/gui/mainwindow.h \
    src/spectrum/spectrumanalyser.h \
    src/basic/animation.h \
    src/basic/color.h \
    src/basic/frame.h \
    src/basic/led.h \
    src/player/ledcontroller.h \
    src/player/player.h \
    src/settings/controlsettings.h \
    src/studio/studio.h \

FORMS += \
    src/gui/mainwindow.ui

include(./include/qextserialport/src/qextserialport.pri)
INCLUDEPATH += /home/bart/prog/ledstrip/include/

LIBS += -L/home/bart/prog/ledstrip/lib/
LIBS += -lportaudio -lasound -lfftw3_omp -lfftw3 -lgomp -lpthread -lm

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS_RELEASE -= -O3

OTHER_FILES +=

OBJECTS_DIR = ./tmp/
MOC_DIR = ./tmp/
UI_DIR = ./src/gui/
