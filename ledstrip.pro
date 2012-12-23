TEMPLATE = app
CONFIG += core gui

SOURCES += \
    gui/mainwindow.cpp \
    audioinput/audioinput.cpp \
    audioinput/userdata.cpp \
    basic/animation.cpp \
    basic/color.cpp \
    basic/frame.cpp \
    basic/led.cpp \
    spectrum/fftw++.cpp \
    spectrum/spectrumanalyser.cpp \
    player/ledcontroller.cpp \
    player/player.cpp \
    settings/controlsettings.cpp \
    studio/studio.cpp \
    main.cpp \
    gui/ledstripemulator.cpp

HEADERS += \
    audioinput/audioinput.h \
    audioinput/userdata.h \
    gui/mainwindow.h \
    spectrum/spectrumanalyser.h \
    basic/animation.h \
    basic/color.h \
    basic/frame.h \
    basic/led.h \
    player/ledcontroller.h \
    player/player.h \
    settings/controlsettings.h \
    studio/studio.h \
    gui/ledstripemulator.h

FORMS += \
    gui/mainwindow.ui

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
unix:QMAKE_CXXFLAGS += -Wall
QMAKE_LFLAGS += -time

OBJECTS_DIR = ./tmp/
MOC_DIR = ./tmp/
UI_DIR = ./src/gui/
