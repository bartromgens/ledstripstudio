TEMPLATE = app
CONFIG += core gui
CONFIG += warn_onwarn_on

SOURCES += \
    src/gui/mainwindow.cpp \
    src/audioinput/audioinput.cpp \
    src/audioinput/userdata.cpp \
    src/basic/color.cpp \
    src/fft/fftw++.cpp \
    src/fft/spectrumanalyser.cpp \
    src/basic/animation.cpp \
    src/basic/frame.cpp \
    src/basic/led.cpp \
    src/player/ledcontroller.cpp \
    src/player/player.cpp \
    src/main.cpp

HEADERS += \
    src/audioinput/audioinput.h \
    src/audioinput/userdata.h \
    src/basic/color.h \
    src/fft/spectrumanalyser.h \
    src/gui/mainwindow.h \
    src/basic/animation.h \
    src/basic/frame.h \
    src/basic/led.h \
    src/player/ledcontroller.h \
    src/player/player.h

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
