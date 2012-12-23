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

FORMS += \
    gui/mainwindow.ui

OTHER_FILES +=

#library header file directories
unix:INCLUDEPATH += /home/bart/prog/ledstrip/include/portaudio/
unix:INCLUDEPATH += /home/bart/prog/ledstrip/include/fftwpp/

win32:INCLUDEPATH += C:/prog/ledstrip/include/portaudio/
win32:INCLUDEPATH += C:/prog/ledstrip/include/fftwpp/
win32:INCLUDEPATH += C:/prog/tools/boost_1_50_0_build/include/

# library locations
unix:LIBS += -L/home/bart/prog/ledstrip/lib/

win32:LIBS += -LC:/prog/ledstrip/lib/vs2010x86/
win32:LIBS += -LC:/prog/tools/boost_1_50_0_build/lib

# libraries
unix:LIBS += -lportaudio -lasound -lfftw3_omp -lfftw3 -lgomp -lpthread -lm
unix:LIBS += -lboost_system152

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
