#!/bin/sh
cd android && \
ant debug && \
adb wait-for-device && \
adb install -r bin/Diplomarbeit-debug.apk && \
adb shell am start -n at.beatthescore.Diplomarbeit/at.beatthescore.Diplomarbeit.QtActivity
