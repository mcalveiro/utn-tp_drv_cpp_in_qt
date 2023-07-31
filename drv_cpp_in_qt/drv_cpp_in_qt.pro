QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        serialportunix.cpp \
        serialportwindows.cpp

HEADERS += \
    serialport.h

# Configuración para la biblioteca de puerto serie
win32: CONFIG += create_prl
unix: CONFIG += create_pc

INCLUDEPATH += .   # Agrega el directorio actual al INCLUDEPATH

win32 {
    LIBS += -lsetupapi       # Enlaza con la biblioteca setupapi.lib en Windows
    DEFINES += _WIN32        # Define la macro _WIN32 para usarla en el código
}

unix {
    # Agrega las bibliotecas requeridas para sistemas Unix (Linux y macOS)
    LIBS += -lpthread
}
