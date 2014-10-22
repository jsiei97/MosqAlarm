TEMPLATE = app
QT += sql
TARGET =

DEPENDPATH  += src
INCLUDEPATH += src
LIBS += -lmosquittopp -lmosquitto

target.path = /usr/local/bin/
INSTALLS += target

# Input
SOURCES += src/main.cpp
SOURCES += src/UnixTime.cpp
SOURCES += src/SQLiteWrapper.cpp
SOURCES += src/MosqConnect.cpp
