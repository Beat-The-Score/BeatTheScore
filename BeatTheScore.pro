TEMPLATE = subdirs

linux {
SUBDIRS += Diplomarbeit
}
!linux {
SUBDIRS += KissFFT SoundTouchLib usb Diplomarbeit
}

CONFIG += ordered

!android {
    CONFIG(release, debug|release)  {
        QMAKE_CXXFLAGS += -O2 -msse -msse2 -m3dnow -fno-strict-aliasing
        QMAKE_CFLAGS += -O2 -msse -msse2 -m3dnow -fno-strict-aliasing
    }
}

OPTIMIZE = $$(DONTGIVEASHIT)
!isEmpty(OPTIMIZE) {
    QMAKE_CXXFLAGS -= -O2
    QMAKE_CFLAGS -= -O2

    QMAKE_CXXFLAGS += -O3
    QMAKE_CFLAGS += -O3
}

android {
    QMAKE_CXXFLAGS += -O3 -fsigned-char
    QMAKE_CFLAGS += -O3 -fsigned-char
    QMAKE_CFLAGS += -march=armv7-a -mtune=cortex-a8 -mfpu=vfpv3-d16 -mfloat-abi=softfp
    QMAKE_CXXFLAGS += -march=armv7-a -mtune=cortex-a8 -mfpu=vfpv3-d16 -mfloat-abi=softfp
    QMAKE_LFLAGS += -march=armv7-a -Wl,--fix-cortex-a8 -Wl,--no-undefined
}

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/res/layout/splash.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/version.xml
