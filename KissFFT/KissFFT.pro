#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T10:59:41
#
#-------------------------------------------------

!linux {
TARGET = kissfft
TEMPLATE = lib
}

SOURCES += \
    $$PWD/kiss_fft.c

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/_kiss_fft_guts.h \
    $$PWD/kiss_fft.h \
    $$PWD/kissfft.hh
