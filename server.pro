QT       += core websockets
QT       -= gui

TARGET =server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    server.cpp

HEADERS += \
    server.h

OTHER_FILES +=
