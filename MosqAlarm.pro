TEMPLATE = app
QT += sql
TARGET =

DEPENDPATH  += src
INCLUDEPATH += src
LIBS += -lmosquittopp -lmosquitto

target.path = /usr/local/bin/
INSTALLS += target

# Input
SOURCES += main.cpp
SOURCES += UnixTime.cpp
SOURCES += SQLiteWrapper.cpp
SOURCES += MosqConnect.cpp
