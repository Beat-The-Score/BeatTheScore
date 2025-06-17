#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T16:58:43
#
#-------------------------------------------------

TARGET = SoundTouchLib
TEMPLATE = lib

SOURCES += \
    source/AAFilter.cpp source/BPMDetect.cpp source/cpu_detect_x86.cpp source/FIFOSampleBuffer.cpp \
    source/FIRFilter.cpp source/mmx_optimized.cpp source/PeakFinder.cpp source/RateTransposer.cpp \
    source/SoundTouch.cpp source/sse_optimized.cpp source/TDStretch.cpp

HEADERS += \
    include/BPMDetect.h \
    include/FIFOSampleBuffer.h \
    include/FIFOSamplePipe.h \
    include/SoundTouch.h \
    include/soundtouch_config.h \
    include/STTypes.h

INCLUDEPATH += $$PWD/source
INCLUDEPATH += $$PWD/include

android {
    QMAKE_CFLAGS += -Wall -D ST_NO_EXCEPTION_HANDLING -fdata-sections -ffunction-sections -marm
    QMAKE_CXXFLAGS += -Wall -D ST_NO_EXCEPTION_HANDLING -fdata-sections -ffunction-sections -marm
}
