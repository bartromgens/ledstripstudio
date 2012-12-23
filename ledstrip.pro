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
    src/gui/ledstripemulator.cpp

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
    src/gui/ledstripemulator.h

FORMS += \
    src/gui/mainwindow.ui

OTHER_FILES +=

INCLUDEPATH += ./src/
DEPENDPATH = $$INCLUDEPATH

#library header file directories
unix:INCLUDEPATH += /home/bart/prog/ledstrip/include/

win32:INCLUDEPATH += C:/prog/ledstrip/include/
win32:INCLUDEPATH += C:/prog/ledstrip/include/fftwpp/
win32:INCLUDEPATH += C:/prog/tools/boost_1_50_0_build/include/

unix:LIBS += -L/home/bart/prog/ledstrip/lib/
win32:LIBS += -LC:/prog/ledstrip/lib/vs2010x86/
unix:LIBS += -lportaudio -lasound -lfftw3_omp -lfftw3 -lgomp -lpthread -lm
win32:LIBS += -lportaudio -llibfftw3-3
win32:LIBS += -LC:/prog/tools/boost_1_50_0_build/lib
unix:LIBS += -lboost_system152

unix:QMAKE_CXXFLAGS += -std=c++0x

#unix:QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS += -Wall
QMAKE_LFLAGS += -time

OBJECTS_DIR = ./tmp/
MOC_DIR = ./tmp/
UI_DIR = ./src/gui/
