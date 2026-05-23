#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T16:58:43
#
#-------------------------------------------------

!linux {
    TARGET = SoundTouchLib
    TEMPLATE = lib
}

SOURCES += \
    $$PWD/source/AAFilter.cpp \
    $$PWD/source/BPMDetect.cpp \
    $$PWD/source/cpu_detect_x86.cpp \
    $$PWD/source/FIFOSampleBuffer.cpp \
    $$PWD/source/FIRFilter.cpp \
    $$PWD/source/mmx_optimized.cpp \
    $$PWD/source/PeakFinder.cpp \
    $$PWD/source/RateTransposer.cpp \
    $$PWD/source/SoundTouch.cpp \
    $$PWD/source/sse_optimized.cpp \
    $$PWD/source/TDStretch.cpp

HEADERS += \
    $$PWD/include/BPMDetect.h \
    $$PWD/include/FIFOSampleBuffer.h \
    $$PWD/include/FIFOSamplePipe.h \
    $$PWD/include/SoundTouch.h \
    $$PWD/include/soundtouch_config.h \
    $$PWD/include/STTypes.h

INCLUDEPATH += $$PWD/source
INCLUDEPATH += $$PWD/include

android {
    QMAKE_CFLAGS += -Wall -D ST_NO_EXCEPTION_HANDLING -fdata-sections -ffunction-sections -marm
    QMAKE_CXXFLAGS += -Wall -D ST_NO_EXCEPTION_HANDLING -fdata-sections -ffunction-sections -marm
}
