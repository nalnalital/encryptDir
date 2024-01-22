TARGET = encryptDir
TEMPLATE = app
DESTROOT=$$PWD/../..
BUILDROOT=$$DESTROOT/builds
message(=== avatars/src/encryptDir/encryptDir.pro ===)
message(TARGET=$$TARGET)

include(../OS_paths.pri)
#include(version.pri)
VERSION = 1.0.0.0
win32{
    ARCH= x86-64
}
android{
    ARCH= arm64-v8a
}
macx{
    ARCH = arm64
}
DESTROOT=$$DESTROOT/$$ARCH
DESTDIR = $$DESTROOT
include (../builds_paths.pri)

QT = core

CONFIG += c++17 cmdline

# === openssl ===

macx{
    INCLUDEPATH += /opt/homebrew/Cellar/openssl@3/3.1.1_1/include
    LIBS += /opt/homebrew/Cellar/openssl@3/3.1.1_1/lib/libssl.3.dylib
    LIBS += /opt/homebrew/Cellar/openssl@3/3.1.1_1/lib/libcrypto.3.dylib
}
win32-msvc*{
    SSLPATH= $$DESTROOT\lib\openssl
    #"C:\Program Files\OpenSSL-Win64"
    message($$SSLPATH)
    INCLUDEPATH += $$SSLPATH\include
    LIBS += -L$$SSLPATH\bin
    LIBS += -L$$SSLPATH\lib
    LIBS += -llibcrypto-3-x64
    LIBS += -llibssl-3-x64
}
win32-g++{
    INCLUDEPATH += C:\Qt\Tools\mingw1120_64\opt\include
    LIBS += -LC:\Qt\Tools\mingw1120_64\opt\bin
    LIBS += -llibcrypto-1_1-x64
    LIBS += -llibssl-1_1-x64
}

# =====================

SOURCES += \
        cipher.cpp \
        main.cpp

HEADERS += \
    cipher.h
